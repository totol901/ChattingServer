/******************************************************************************
파일명	: FrameImage.h
목적		: 프래임 이미지 추상화함
******************************************************************************/
#pragma once

class Image;
class FrameImage : public Image
{
private:
	typedef struct tagFrameImageInfo
	{
		UINT maxFrameX;
		UINT maxFrameY;
		float frameWidth;
		float frameHeight;

		tagFrameImageInfo()
		{
			maxFrameX = 0;
			maxFrameY = 0;
			frameWidth = 0.0f;
			frameHeight = 0.0f;
		}
	} FRAMEIMAGE_INFO, *LPFRAMEIMAGE_INFO;

	LPFRAMEIMAGE_INFO m_pFrameImageInfo;

public:
	FrameImage(wstring resourceNum);
	~FrameImage();

	/****************************************************************************
	함수명	: init
	설명		: 프래임 이미지를 초기화 해줌
	*****************************************************************************/
	HRESULT init(IWICImagingFactory* wicFactory, const TCHAR* fileName, float x, float y, float scaleX, float scaleY, UINT MaxframeX, UINT MaxframeY, BOOL trans = TRUE, D2D1::ColorF transColor = D2D1::ColorF(1, 0, 1));

	/****************************************************************************
	함수명	: Render
	설명		: 프래임 이미지를 랜더해줌
	*****************************************************************************/
	virtual void Render(float destX, float destY, UINT frameX, UINT frameY);


	//get set

	//중점 X
	virtual inline float getCenterX()
	{
		return m_pImageInfo->x + (m_pFrameImageInfo->frameWidth* 0.5f);
	}

	//중점 Y
	virtual inline float getCenterY()
	{
		return m_pImageInfo->y + (m_pFrameImageInfo->frameHeight * 0.5f);
	}

	//이미지 가로
	inline float getFrameWidth(void) { return m_pFrameImageInfo->frameWidth; }
	inline void setFrameWidth(float frameWidth) { m_pFrameImageInfo->frameWidth = frameWidth; }

	//이미지 세로
	inline float getFrameHeight(void) { return m_pFrameImageInfo->frameHeight; }
	inline void setFrameHeight(float frameHeight) { m_pFrameImageInfo->frameHeight = frameHeight; }
};
