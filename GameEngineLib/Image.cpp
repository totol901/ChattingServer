#include "stdafx.h"
#include "Image.h"

Image::Image(wstring resourceNum)
	:GameResource(resourceNum),
	m_pImageInfo(nullptr)
{
}

Image::~Image()
{
	Release();
	SAFE_DELETE(m_pImageInfo);
}

HRESULT Image::init(IWICImagingFactory* wicFactory, const TCHAR * fileName, float x, float y, float scaleX, float scaleY, BOOL trans, D2D1::ColorF transColor)
{
	m_pImageInfo = new IMAGE_INFO;
	m_pImageInfo->x = x;
	m_pImageInfo->y = y;
	m_pImageInfo->scaleX = scaleX;
	m_pImageInfo->scaleY = scaleY;
	m_pImageInfo->loadType = LOAD_NORMAL_IMAGE_FILE;

	//���ڴ� ����
	IWICBitmapDecoder *pDecoder = nullptr;

	HRESULT hr;
	//���Ϸκ��� �̹��� �ҷ��´�
	hr = wicFactory->CreateDecoderFromFilename(
		fileName,                      // ���ڴ��� �̹��� ����
		NULL,								// �ʿ� ������ NULL
		GENERIC_READ,						// �б⸸ �Ѵ�.
		WICDecodeMetadataCacheOnDemand,		// ��û�� �о���� ���� ĳ��ȭ��
		&pDecoder							// ���ڴ��� �־���
	);

	// ù��° �����ӿ� �־��ֱ����� ������ ����
	IWICBitmapFrameDecode *pFrame = nullptr;

	//������ �޾ƿ´�
	if (SUCCEEDED(hr))
	{
		hr = pDecoder->GetFrame(0, &pFrame);
	}
	else
	{
		Assert("������ �ޱ� ����");
		return hr;
	}

	//XXX : ������ ó���� �ȵ�.. ������ ������� �ؾߵɵ�
	//������ ���ش�
	UINT width, height = 0;
	pFrame->GetSize(&width, &height);
	m_pImageInfo->width = width;
	m_pImageInfo->height = height; 
	//IWICBitmapScaler* pIScaler = nullptr;
	//hr = wicFactory->CreateBitmapScaler(&pIScaler);
	//if (SUCCEEDED(hr))
	//{
	//	hr = pIScaler->Initialize(
	//		pFrame,                    // Bitmap source to scale.
	//		(UINT)(width * scaleX),                         // Scale width to half of original.
	//		(UINT)(height * scaleY),                        // Scale height to half of original.
	//		WICBitmapInterpolationModeFant);   // Use Fant mode interpolation.
	//}
	//else
	//{
	//	Assert(0);
	//	return hr;
	//}


	//������ ������ش�.
	if (SUCCEEDED(hr))
	{
		SAFE_RELEASE(m_pImageInfo->pIWICFormatConverter);
		hr = wicFactory->CreateFormatConverter(&m_pImageInfo->pIWICFormatConverter);
	}
	else
	{
		Assert("������ ���� ����");
		return hr;
	}

	//�����͸� �������� �̿��Ͽ� init���ش�.
	if (SUCCEEDED(hr))
	{
		hr = m_pImageInfo->pIWICFormatConverter->Initialize(
			pFrame,                          // Input bitmap to convert
			GUID_WICPixelFormat32bppPBGRA,   // Destination pixel format
			WICBitmapDitherTypeNone,         // Specified dither pattern
			NULL,                            // Specify a particular palette 
			0.f,                             // Alpha threshold
			WICBitmapPaletteTypeCustom       // Palette translation type
		);
	}
	else
	{
		Assert("������ �̿��Ͽ� ������ ���� ����");
		return hr;
	}

	//����̽����� ��Ʈ���� ��������쵵 �����Ƿ� �ٽ� ������ش�.
	if (m_pImageInfo->pIWICFormatConverter && !m_pImageInfo->pID2D1Bitmap)
	{
		D2D_RENDERTARGET->CreateBitmapFromWicBitmap(
			m_pImageInfo->pIWICFormatConverter,
			NULL,
			&m_pImageInfo->pID2D1Bitmap);
	}
	else
	{
		Assert("d2d�� bitmap ���� ����");
		return hr;
	}

	//Ʈ���� �÷� ������
	if (trans)
	{
		hr = TranslateColorFromBitmap(wicFactory,  transColor);
		if (FAILED(hr))
		{
			Assert("Ʈ���� �÷� ������ ����");
			return hr;
		}
	}

	//SAFE_RELEASE(pIScaler);
	SAFE_RELEASE(pDecoder);
	SAFE_RELEASE(pFrame);

	return hr;
}


HRESULT Image::TranslateColorFromBitmap(IWICImagingFactory* wicFactory, D2D1::ColorF transColor)
{
	//transColor �����ش�!
	//��Ʈ�� �޾ƿ������� ����
	IWICBitmap* pColorKeyedBmp = NULL;
	//��Ʈ�� �ʺ�, ����
	UINT    uBmpW = 0;
	UINT    uBmpH = 0;
	//�̹��� ���Ϸ� ���� ���� ��Ʈ�� ������ �ʺ�, ���̸� �޾ƿ�
	m_pImageInfo->pIWICFormatConverter->GetSize(&uBmpW, &uBmpH);

	//��Ʈ�� Lock�� �ɱ� ���� Rect
	WICRect rcLock = { 0, 0, (int)uBmpW, (int)uBmpH };
	//pColorKeyedBmp�� �ҷ��� ��Ʈ���� �־���
	HRESULT hr = wicFactory->CreateBitmapFromSource(m_pImageInfo->pIWICFormatConverter,
		WICBitmapCacheOnLoad,
		&pColorKeyedBmp);

	//��Ʈ�� ũ�⸸ŭ �� �ɾ���
	IWICBitmapLock* pBitmapLock = NULL;
	hr = pColorKeyedBmp->Lock(&rcLock, WICBitmapLockRead, &pBitmapLock);
	if (FAILED(hr))
	{
		SAFE_RELEASE(pColorKeyedBmp);
		return hr;
	}

	UINT  uPixel = 0;
	UINT  cbBufferSize = 0;
	UINT  cbStride = 0;
	BYTE* pPixelBuffer = NULL;
	
	//��Ʈ�� ���� ���ؼ� �ȼ� ��������
	//��Ʈ���� Row ����Ʈ ũ�⸦ �޾ƿ�
	hr = pBitmapLock->GetStride(&cbStride);
	if (SUCCEEDED(hr))
	{
		//��Ʈ���� �ȼ� ������ ������ �����Ϳ� ũ�⸦ �޾ƿ�
		hr = pBitmapLock->GetDataPointer(&cbBufferSize, &pPixelBuffer);
		if (SUCCEEDED(hr))
		{
			LPBYTE pPixel = pPixelBuffer;

			for (UINT row = 0; row < uBmpH; row++)
			{
				//�ȼ� ������ ������ ���� �ٷ�
				pPixel = pPixelBuffer + (row * cbStride);

				//Row���� ����Ÿ �ȼ��� ������
				for (UINT uCol = 0; uCol < uBmpW; uCol++)
				{

					uPixel = *((LPUINT)pPixel);

					D2D1::ColorF pix = uPixel;

					if ((pix.r == transColor.r) &&
						(pix.g == transColor.g) &&
						(pix.b == transColor.b))
					{
						*((LPUINT)pPixel) = 0;
					}
					pPixel += sizeof(uPixel);
				}
			}
		}
	}

	//WicStruct�� �ٲ� ��Ʈ�� �����
	D2D1_RECT_U rc = { (UINT)rcLock.X, (UINT)rcLock.Y, (UINT)rcLock.Width, (UINT)rcLock.Height };
	m_pImageInfo->pID2D1Bitmap->CopyFromMemory(&rc, pPixelBuffer, cbStride);

	//�� ��Ʈ�� ������
	SAFE_RELEASE(pBitmapLock);
	SAFE_RELEASE(pColorKeyedBmp);

	return hr;
}

void Image::Release(void)
{
	SAFE_RELEASE(m_pImageInfo->pID2D1Bitmap);
	SAFE_RELEASE(m_pImageInfo->pIWICFormatConverter);
}

void Image::Render(float destX, float destY)
{
	//��Ʈ�� �ҽǉ�ٸ� �ٽ� ����
	if (m_pImageInfo->pIWICFormatConverter && !m_pImageInfo->pID2D1Bitmap)
	{
		DIRECT2D->GetBackBufferTarget()->CreateBitmapFromWicBitmap(
			m_pImageInfo->pIWICFormatConverter,
			NULL,
			&m_pImageInfo->pID2D1Bitmap);
	}

	DIRECT2D->GetBackBufferTarget()->BeginDraw();
	//�̹��� �׸���
	D2D1_SIZE_F image_size = m_pImageInfo->pID2D1Bitmap->GetSize();				//�̹��� ���� ���ؼ� �񽺸ʿ��� �̹��� ������ �޾ƿ´�.
	D2D1_RECT_F dxArea = D2D1::RectF(destX, destY, image_size.width + destX, image_size.height + destY);
	DIRECT2D->GetBackBufferTarget()->DrawBitmap(m_pImageInfo->pID2D1Bitmap,
		dxArea);	//�̹��� ũ�⸸ŭ �׷��ش�.
	DIRECT2D->GetBackBufferTarget()->EndDraw();
}

void Image::RenderDCTarget(float destX, float destY)
{
	////��Ʈ�� �ҽǉ�ٸ� �ٽ� ����
	//if (m_pImageInfo->pIWICFormatConverter && !m_pImageInfo->pID2D1Bitmap)
	//{
	//	DIRECT2D->GetBackBufferTarget()->CreateBitmapFromWicBitmap(
	//		m_pImageInfo->pIWICFormatConverter,
	//		NULL,
	//		&m_pImageInfo->pID2D1Bitmap);
	//}
	//
	//DIRECT2D->GetDCRenderTarget()->BeginDraw();
	////�̹��� �׸���
	//D2D1_SIZE_F image_size = m_pImageInfo->pID2D1Bitmap->GetSize();				//�̹��� ���� ���ؼ� �񽺸ʿ��� �̹��� ������ �޾ƿ´�.
	//D2D1_RECT_F dxArea = D2D1::RectF(destX, destY, image_size.width + destX, image_size.height + destY);
	//DIRECT2D->GetDCRenderTarget()->DrawBitmap(m_pImageInfo->pID2D1Bitmap,
	//	dxArea);	//�̹��� ũ�⸸ŭ �׷��ش�.
	//DIRECT2D->GetDCRenderTarget()->EndDraw();
}
