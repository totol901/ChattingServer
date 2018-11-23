/******************************************************************************
���ϸ�	: ImageManager.h
����		: �̹������� �����ϴ� ��ü
******************************************************************************/
#pragma once

class Image;
class ImageManager
{
	friend ResourceManager;
private:
	ImageManager(const ImageManager&) = delete;
	ImageManager(const ImageManager&&) = delete;
	ImageManager& operator=(const ImageManager&) = delete;
	ImageManager& operator=(const ImageManager&&) = delete;
	ImageManager();
	~ImageManager();

	IWICImagingFactory*		m_pWICFactory;

	map<wstring, Image*> m_mapImages;

	BOOL InsertImage(const wstring& num, Image* image);

public:
	HRESULT Init();
	void Destroy();

	/****************************************************************************
	�Լ���	: MakeFrameImageAndInsertMap
	����		: ������ �̹����� ����� �ʿ� �־���
	*****************************************************************************/
	HRESULT MakeFrameImageAndInsertMap(const wstring& imageID, const TCHAR * fileName, UINT maxFrameX, UINT maxFrameY, BOOL trans = TRUE, D2D1::ColorF transColor = D2D1::ColorF(1.0f,0.0f,1.0f), float x = 0.0f, float y = 0.0f, float scaleX = 1.0f, float scaleY = 1.0f);
	
	/****************************************************************************
	�Լ���	: MakeImageAndInsertMap
	����		: �̹����� ����� �ʿ� �־���
	*****************************************************************************/
	HRESULT MakeImageAndInsertMap(const wstring& imageID, const TCHAR * fileName, BOOL trans = TRUE, D2D1::ColorF transColor = D2D1::ColorF(1.0f, 0.0f, 1.0f), float x = 0.0f, float y = 0.0f, float scaleX = 1.0f, float scaleY = 1.0f);
	
	/****************************************************************************
	�Լ���	: DeleteImage
	����		: �̹����� ������
	*****************************************************************************/
	BOOL DeleteImage(const wstring& num);

	/****************************************************************************
	�Լ���	: FindImage
	����		: �̹����� ã�Ƽ� ��������
	*****************************************************************************/
	Image* FindImage(const wstring& num);
};
