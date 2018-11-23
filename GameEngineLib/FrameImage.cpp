#include "stdafx.h"
#include "FrameImage.h"

FrameImage::FrameImage(wstring resourceNum)
	:Image(resourceNum),
	m_pFrameImageInfo(nullptr)
{
}

FrameImage::~FrameImage()
{
	SAFE_DELETE(m_pFrameImageInfo);
}

HRESULT FrameImage::init(IWICImagingFactory * wicFactory, const TCHAR * fileName, float x, float y, float scaleX, float scaleY, UINT MaxframeX, UINT MaxframeY, BOOL trans, D2D1::ColorF transColor)
{
	HRESULT hr = Image::init(wicFactory, fileName, x, y, scaleX, scaleY, trans, transColor);
	Image::m_pImageInfo->loadType = LOAD_FRAME_IMAGE_FILE;

	if (FAILED(hr))
	{
		return hr;
	}
	
	//������ ���� ���� �ʱ�ȭ
	D2D1_SIZE_F size = Image::m_pImageInfo->pID2D1Bitmap->GetSize();

	m_pFrameImageInfo = new FRAMEIMAGE_INFO;
	m_pFrameImageInfo->maxFrameX = MaxframeX;
	m_pFrameImageInfo->maxFrameY = MaxframeY;
	m_pFrameImageInfo->frameWidth = size.width / (float)MaxframeX;
	m_pFrameImageInfo->frameHeight = size.height / (float)MaxframeY;

	return hr;
}

void FrameImage::Render(float destX, float destY, UINT frameX, UINT frameY)
{
	//��Ʈ�� �ҽǉ�ٸ� �ٽ� ����
	if (Image::m_pImageInfo->pIWICFormatConverter && 
		!Image::m_pImageInfo->pID2D1Bitmap)
	{
		DIRECT2D->GetBackBufferTarget()->CreateBitmapFromWicBitmap(
			Image::m_pImageInfo->pIWICFormatConverter,
			NULL,
			&Image::m_pImageInfo->pID2D1Bitmap);
	}

	//�̹��� �׸���
	D2D1_SIZE_F image_size = Image::m_pImageInfo->pID2D1Bitmap->GetSize();				//�̹��� ���� ���ؼ� �񽺸ʿ��� �̹��� ������ �޾ƿ´�.
	D2D1_RECT_F sourArea = D2D1::RectF(
		m_pFrameImageInfo->frameWidth * (frameX),
		m_pFrameImageInfo->frameHeight * (frameY),
		m_pFrameImageInfo->frameWidth * (frameX + 1),
		m_pFrameImageInfo->frameHeight * (frameY + 1)
	);

	if (image_size.width < sourArea.right)
	{
		Assert("�׸� �̹��� ũ�� �ʰ�");
	}
	if (image_size.height < sourArea.bottom)
	{
		Assert("�׸� �̹��� ũ�� �ʰ�");
	}
	
	D2D1_RECT_F desArea = D2D1::RectF(
		destX,
		destY,
		destX + m_pFrameImageInfo->frameWidth,
		destY + m_pFrameImageInfo->frameHeight);

	DIRECT2D->GetBackBufferTarget()->BeginDraw();
	DIRECT2D->GetBackBufferTarget()->DrawBitmap(Image::m_pImageInfo->pID2D1Bitmap,
		desArea, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		sourArea);
	DIRECT2D->GetBackBufferTarget()->BeginDraw();
}
