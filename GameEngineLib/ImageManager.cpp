#include "stdafx.h"
#include "ImageManager.h"

ImageManager::ImageManager()
	:m_pWICFactory(nullptr)
{
}

ImageManager::~ImageManager()
{
}

HRESULT ImageManager::Init()
{
	HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory,
		NULL, CLSCTX_INPROC_SERVER, //CLSCTX_INPROC_SERVER : 개체가 동적 링크 라이브러리 (DLL)로서 처리 되어 애플리케이션의 처리의 일부로서 실행되는 것을 나타낸다.
		IID_PPV_ARGS(&m_pWICFactory));//IID_PPV_ARGS 매크로 : 인터페이스 포인터 유형에 따라 자동으로 ID값을 제공하여 검색함

	return hr;
}

void ImageManager::Destroy()
{
	SAFE_RELEASE(m_pWICFactory);

	for (auto iter = m_mapImages.begin();
		iter != m_mapImages.end(); iter++)
	{
		SAFE_DELETE(iter->second);
	}
	m_mapImages.clear();
}

HRESULT ImageManager::MakeFrameImageAndInsertMap(const wstring & imageID, const TCHAR * fileName, UINT maxFrameX, UINT maxFrameY, BOOL trans, D2D1::ColorF transColor, float x, float y, float scaleX, float scaleY)
{
	//이미지
	FrameImage* image = new FrameImage(imageID);
	HRESULT hr = image->init(m_pWICFactory, fileName, x, y, scaleX, scaleY, maxFrameX, maxFrameY, trans, transColor);

	if (FAILED(hr))
	{
		return hr;
	}

	//맵어 넣어줌
	if (!InsertImage(imageID, image))
	{
		return E_FAIL;
	}

	return hr;
}

HRESULT ImageManager::MakeImageAndInsertMap(const wstring& imageID, const TCHAR * fileName, BOOL trans, D2D1::ColorF transColor, float x, float y, float scaleX, float scaleY)
{
	//이미지
	Image* image = new Image(imageID);
	HRESULT hr = image->init(m_pWICFactory, fileName, x, y, scaleX, scaleY, trans, transColor);
	
	if (FAILED(hr))
	{
		return hr;
	}

	//맵어 넣어줌
	if (!InsertImage(imageID, image))
	{
		return E_FAIL;
	}

	return hr;
}

BOOL ImageManager::InsertImage(const wstring& num, Image * image)
{
	auto iter = m_mapImages.find(num);
	if (iter == m_mapImages.end())
	{
		m_mapImages.insert(make_pair(num, image));
		
		return TRUE;
	}

	return FALSE;
}

BOOL ImageManager::DeleteImage(const wstring & num)
{
	auto iter = m_mapImages.find(num);
	if (iter == m_mapImages.end())
	{
		Assert("이미지 삭제 실패");
		return FALSE;
	}

	SAFE_DELETE(iter->second);
	m_mapImages.erase(iter);

	return TRUE;
}

Image * ImageManager::FindImage(const wstring & num)
{
	auto iter = m_mapImages.find(num);

	if (iter == m_mapImages.end())
	{
		Assert("이미지 찾기 실패");
		return nullptr;
	}

	return iter->second;
}
