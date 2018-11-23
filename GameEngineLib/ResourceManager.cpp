#include "stdafx.h"
#include "ResourceManager.h"

ResourceManager::ResourceManager()
	:m_pImageManager(nullptr),
	m_pTextManager(nullptr)
{
}

ResourceManager::~ResourceManager()
{
	SAFE_DELETE(m_pTextManager);
	SAFE_DELETE(m_pImageManager);
}

HRESULT ResourceManager::Init()
{
	m_pImageManager = new ImageManager;
	m_pTextManager = new TextManager;

	HRESULT hr = m_pImageManager->Init();

	if (SUCCEEDED(hr))
	{
		hr = m_pTextManager->Init();
	}

	return hr;
}

void ResourceManager::Destroy()
{
	m_pTextManager->Destory();
	m_pImageManager->Destroy();
}
