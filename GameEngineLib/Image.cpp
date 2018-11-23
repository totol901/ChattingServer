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

	//디코더 선언
	IWICBitmapDecoder *pDecoder = nullptr;

	HRESULT hr;
	//파일로부터 이미지 불러온다
	hr = wicFactory->CreateDecoderFromFilename(
		fileName,                      // 디코더할 이미지 파일
		NULL,								// 필요 없으니 NULL
		GENERIC_READ,						// 읽기만 한다.
		WICDecodeMetadataCacheOnDemand,		// 요청시 읽어오기 위해 캐쉬화함
		&pDecoder							// 디코더에 넣어줌
	);

	// 첫번째 프래임에 넣어주기위해 프래임 선언
	IWICBitmapFrameDecode *pFrame = nullptr;

	//프래임 받아온다
	if (SUCCEEDED(hr))
	{
		hr = pDecoder->GetFrame(0, &pFrame);
	}
	else
	{
		Assert("프래임 받기 실패");
		return hr;
	}

	//XXX : 스케일 처리가 안됨.. 스케일 계산으로 해야될듯
	//스케일 해준다
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


	//컨버터 만들어준다.
	if (SUCCEEDED(hr))
	{
		SAFE_RELEASE(m_pImageInfo->pIWICFormatConverter);
		hr = wicFactory->CreateFormatConverter(&m_pImageInfo->pIWICFormatConverter);
	}
	else
	{
		Assert("컨버터 생성 실패");
		return hr;
	}

	//컨버터를 프래임을 이용하여 init해준다.
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
		Assert("프래임 이용하여 컨버터 생성 실패");
		return hr;
	}

	//디바이스에서 비트맵이 없어질경우도 있으므로 다시 만들어준다.
	if (m_pImageInfo->pIWICFormatConverter && !m_pImageInfo->pID2D1Bitmap)
	{
		D2D_RENDERTARGET->CreateBitmapFromWicBitmap(
			m_pImageInfo->pIWICFormatConverter,
			NULL,
			&m_pImageInfo->pID2D1Bitmap);
	}
	else
	{
		Assert("d2d용 bitmap 생성 실패");
		return hr;
	}

	//트렌스 컬러 날려줌
	if (trans)
	{
		hr = TranslateColorFromBitmap(wicFactory,  transColor);
		if (FAILED(hr))
		{
			Assert("트랜스 컬러 날리기 실패");
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
	//transColor 날려준다!
	//비트맵 받아오기위해 선언
	IWICBitmap* pColorKeyedBmp = NULL;
	//비트맵 너비, 높이
	UINT    uBmpW = 0;
	UINT    uBmpH = 0;
	//이미지 파일로 부터 뽑은 비트맵 파일의 너비, 높이를 받아옴
	m_pImageInfo->pIWICFormatConverter->GetSize(&uBmpW, &uBmpH);

	//비트맵 Lock을 걸기 위한 Rect
	WICRect rcLock = { 0, 0, (int)uBmpW, (int)uBmpH };
	//pColorKeyedBmp에 불러온 비트맵을 넣어줌
	HRESULT hr = wicFactory->CreateBitmapFromSource(m_pImageInfo->pIWICFormatConverter,
		WICBitmapCacheOnLoad,
		&pColorKeyedBmp);

	//비트맵 크기만큼 락 걸어줌
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
	
	//비트맵 락을 통해서 픽셀 변경해줌
	//비트맵의 Row 바이트 크기를 받아옴
	hr = pBitmapLock->GetStride(&cbStride);
	if (SUCCEEDED(hr))
	{
		//비트맵의 픽셀 데이터 시작점 포인터와 크기를 받아옴
		hr = pBitmapLock->GetDataPointer(&cbBufferSize, &pPixelBuffer);
		if (SUCCEEDED(hr))
		{
			LPBYTE pPixel = pPixelBuffer;

			for (UINT row = 0; row < uBmpH; row++)
			{
				//픽셀 한줄이 끝나면 다음 줄로
				pPixel = pPixelBuffer + (row * cbStride);

				//Row줄의 마젠타 픽셀을 날려줌
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

	//WicStruct에 바꾼 비트맵 담아줌
	D2D1_RECT_U rc = { (UINT)rcLock.X, (UINT)rcLock.Y, (UINT)rcLock.Width, (UINT)rcLock.Height };
	m_pImageInfo->pID2D1Bitmap->CopyFromMemory(&rc, pPixelBuffer, cbStride);

	//락 비트맵 날려줌
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
	//비트맵 소실됬다면 다시 만듬
	if (m_pImageInfo->pIWICFormatConverter && !m_pImageInfo->pID2D1Bitmap)
	{
		DIRECT2D->GetBackBufferTarget()->CreateBitmapFromWicBitmap(
			m_pImageInfo->pIWICFormatConverter,
			NULL,
			&m_pImageInfo->pID2D1Bitmap);
	}

	DIRECT2D->GetBackBufferTarget()->BeginDraw();
	//이미지 그리기
	D2D1_SIZE_F image_size = m_pImageInfo->pID2D1Bitmap->GetSize();				//이미지 띄우기 위해서 비스맵에서 이미지 사이즈 받아온다.
	D2D1_RECT_F dxArea = D2D1::RectF(destX, destY, image_size.width + destX, image_size.height + destY);
	DIRECT2D->GetBackBufferTarget()->DrawBitmap(m_pImageInfo->pID2D1Bitmap,
		dxArea);	//이미지 크기만큼 그려준다.
	DIRECT2D->GetBackBufferTarget()->EndDraw();
}

void Image::RenderDCTarget(float destX, float destY)
{
	////비트맵 소실됬다면 다시 만듬
	//if (m_pImageInfo->pIWICFormatConverter && !m_pImageInfo->pID2D1Bitmap)
	//{
	//	DIRECT2D->GetBackBufferTarget()->CreateBitmapFromWicBitmap(
	//		m_pImageInfo->pIWICFormatConverter,
	//		NULL,
	//		&m_pImageInfo->pID2D1Bitmap);
	//}
	//
	//DIRECT2D->GetDCRenderTarget()->BeginDraw();
	////이미지 그리기
	//D2D1_SIZE_F image_size = m_pImageInfo->pID2D1Bitmap->GetSize();				//이미지 띄우기 위해서 비스맵에서 이미지 사이즈 받아온다.
	//D2D1_RECT_F dxArea = D2D1::RectF(destX, destY, image_size.width + destX, image_size.height + destY);
	//DIRECT2D->GetDCRenderTarget()->DrawBitmap(m_pImageInfo->pID2D1Bitmap,
	//	dxArea);	//이미지 크기만큼 그려준다.
	//DIRECT2D->GetDCRenderTarget()->EndDraw();
}
