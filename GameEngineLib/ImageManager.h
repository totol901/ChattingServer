/******************************************************************************
파일명	: ImageManager.h
목적		: 이미지들을 관리하는 객체
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
	함수명	: MakeFrameImageAndInsertMap
	설명		: 프래임 이미지를 만들고 맵에 넣어줌
	*****************************************************************************/
	HRESULT MakeFrameImageAndInsertMap(const wstring& imageID, const TCHAR * fileName, UINT maxFrameX, UINT maxFrameY, BOOL trans = TRUE, D2D1::ColorF transColor = D2D1::ColorF(1.0f,0.0f,1.0f), float x = 0.0f, float y = 0.0f, float scaleX = 1.0f, float scaleY = 1.0f);
	
	/****************************************************************************
	함수명	: MakeImageAndInsertMap
	설명		: 이미지를 만들고 맵에 넣어줌
	*****************************************************************************/
	HRESULT MakeImageAndInsertMap(const wstring& imageID, const TCHAR * fileName, BOOL trans = TRUE, D2D1::ColorF transColor = D2D1::ColorF(1.0f, 0.0f, 1.0f), float x = 0.0f, float y = 0.0f, float scaleX = 1.0f, float scaleY = 1.0f);
	
	/****************************************************************************
	함수명	: DeleteImage
	설명		: 이미지를 삭제함
	*****************************************************************************/
	BOOL DeleteImage(const wstring& num);

	/****************************************************************************
	함수명	: FindImage
	설명		: 이미지를 찾아서 리턴해줌
	*****************************************************************************/
	Image* FindImage(const wstring& num);
};
