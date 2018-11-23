/******************************************************************************
���ϸ�	: Image.h
����		: �̹����� �߻�ȭ
******************************************************************************/
#pragma once

class Image : public GameResource
{
protected:
	enum IMAGE_LOAD_KIND
	{
		LOAD_NONE,
		LOAD_EMPTY,		//�� ��Ʈ�� ����
		LOAD_RESOURCE,	//���ҽ� ���� pk3, pak
		LOAD_NORMAL_IMAGE_FILE,		//�̹��� ����
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

	//��Ʈ���� Ʈ���� �÷��� �����ִ� �Լ�
	HRESULT TranslateColorFromBitmap(IWICImagingFactory* wicFactory,  D2D1::ColorF transColor);
	
public:
	Image(wstring resourceNum);
	virtual ~Image();

	/****************************************************************************
	�Լ���	: init
	����		: �̹����� �ʱ�ȭ ����
	*****************************************************************************/
	HRESULT init(IWICImagingFactory* wicFactory, const TCHAR* fileName, float x, float y, float scaleX, float scaleY,
		BOOL trans = TRUE, D2D1::ColorF transColor = D2D1::ColorF(1,0,1));

	void Release(void);

	/****************************************************************************
	�Լ���	: Render
	����		: �̹����� ��������
	*****************************************************************************/
	void Render(float destX, float destY);

	/****************************************************************************
	�Լ���	: Render
	����		: ������ �̹����� ��������
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

	//���� X
	virtual inline float getCenterX()
	{
		return m_pImageInfo->x + (m_pImageInfo->width * 0.5f) ;
	}

	//���� Y
	virtual inline float getCenterY()
	{
		return m_pImageInfo->y + (m_pImageInfo->height * 0.5f);
	}

	//�̹��� ����
	inline float getWidth(void) { return m_pImageInfo->width; }
	inline void setWidth(float _width) { m_pImageInfo->width = _width; }

	//�̹��� ����
	inline float getHeight(void) { return m_pImageInfo->height; }
	inline void setHeight(float _height) { m_pImageInfo->height = _height; }
};
