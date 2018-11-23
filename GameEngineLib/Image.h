/******************************************************************************
파일명	: Image.h
목적		: 이미지를 추상화
******************************************************************************/
#pragma once

class Image : public GameResource
{
protected:
	enum IMAGE_LOAD_KIND
	{
		LOAD_NONE,
		LOAD_EMPTY,		//빈 비트맵 파일
		LOAD_RESOURCE,	//리소스 파일 pk3, pak
		LOAD_NORMAL_IMAGE_FILE,		//이미지 파일
		LOAD_FRAME_IMAGE_FILE
	};

	typedef struct tagImageInfo
	{
		IWICFormatConverter* pIWICFormatConverter;
		ID2D1Bitmap* pID2D1Bitmap;
		float x;
		float y;
		float width;
		float height;
		float scaleX;
		float scaleY;
		IMAGE_LOAD_KIND loadType;

		tagImageInfo()
		{
			pIWICFormatConverter = nullptr;
			pID2D1Bitmap = nullptr;
			x = 0.0f;
			y = 0.0f;
			width = 0.0f;
			height = 0.0f;
			scaleX = 0.0f;
			scaleY = 0.0f;
			loadType = LOAD_NONE;
		}
	}IMAGE_INFO, *LPIMAGE_INFO;

	LPIMAGE_INFO m_pImageInfo;

	//비트맵의 트랜스 컬러를 날려주는 함수
	HRESULT TranslateColorFromBitmap(IWICImagingFactory* wicFactory,  D2D1::ColorF transColor);
	
public:
	Image(wstring resourceNum);
	virtual ~Image();

	/****************************************************************************
	함수명	: init
	설명		: 이미지를 초기화 해줌
	*****************************************************************************/
	HRESULT init(IWICImagingFactory* wicFactory, const TCHAR* fileName, float x, float y, float scaleX, float scaleY,
		BOOL trans = TRUE, D2D1::ColorF transColor = D2D1::ColorF(1,0,1));

	void Release(void);

	/****************************************************************************
	함수명	: Render
	설명		: 이미지를 랜더해줌
	*****************************************************************************/
	void Render(float destX, float destY);

	/****************************************************************************
	함수명	: Render
	설명		: 프래임 이미지를 랜더해줌
	*****************************************************************************/
	virtual void Render(float destX, float destY, UINT frameX, UINT frameY) {}
	
	void RenderDCTarget(float destX, float destY);

	//get set
	
	ID2D1Bitmap* GetBitmap() { return m_pImageInfo->pID2D1Bitmap; }
	inline void setX(float x) { m_pImageInfo->x = x; }
	inline float getX() { return m_pImageInfo->x; }

	inline void setY(float y) { m_pImageInfo->y = y; }
	inline float getY() { return m_pImageInfo->y; }

	inline void setCenter(float x, float y)
	{
		m_pImageInfo->x = x - (m_pImageInfo->width * 0.5f);
		m_pImageInfo->y = y - (m_pImageInfo->height * 0.5f);
	}

	//중점 X
	virtual inline float getCenterX()
	{
		return m_pImageInfo->x + (m_pImageInfo->width * 0.5f) ;
	}

	//중점 Y
	virtual inline float getCenterY()
	{
		return m_pImageInfo->y + (m_pImageInfo->height * 0.5f);
	}

	//이미지 가로
	inline float getWidth(void) { return m_pImageInfo->width; }
	inline void setWidth(float _width) { m_pImageInfo->width = _width; }

	//이미지 세로
	inline float getHeight(void) { return m_pImageInfo->height; }
	inline void setHeight(float _height) { m_pImageInfo->height = _height; }
};
