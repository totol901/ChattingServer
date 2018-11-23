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
		NULL, CLSCTX_INPROC_SERVER, //CLSCTX_INPROC_SERVER : ��ü�� ���� ��ũ ���̺귯�� (DLL)�μ� ó�� �Ǿ� ���ø����̼��� ó���� �Ϻημ� ����Ǵ� ���� ��Ÿ����.
		IID_PPV_ARGS(&m_pWICFactory));//IID_PPV_ARGS ��ũ�� : �������̽� ������ ������ ���� �ڵ����� ID���� �����Ͽ� �˻���

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
	//�̹���
	FrameImage* image = new FrameImage(imageID);
	HRESULT hr = image->init(m_pWICFactory, fileName, x, y, scaleX, scaleY, maxFrameX, maxFrameY, trans, transColor);

	if (FAILED(hr))
	{
		return hr;
	}

	//�ʾ� �־���
	if (!InsertImage(imageID, image))
	{
		return E_FAIL;
	}

	return hr;
}

HRESULT ImageManager::MakeImageAndInsertMap(const wstring& imageID, const TCHAR * fileName, BOOL trans, D2D1::ColorF transColor, float x, float y, float scaleX, float scaleY)
{
	//�̹���
	Image* image = new Image(imageID);
	HRESULT hr = image->init(m_pWICFactory, fileName, x, y, scaleX, scaleY, trans, transColor);
	
	if (FAILED(hr))
	{
		return hr;
	}

	//�ʾ� �־���
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
		Assert("�̹��� ���� ����");
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
		Assert("�̹��� ã�� ����");
		return nullptr;
	}

	return iter->second;
}
