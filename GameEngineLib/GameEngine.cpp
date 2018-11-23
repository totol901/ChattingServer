#include "stdafx.h"
#include "GameEngine.h"

GameEngine::GameEngine()
	:m_pGrapics(nullptr),
	m_pResourceManager(nullptr)
{
}

GameEngine::~GameEngine()
{
	SAFE_DELETE(m_pResourceManager);
	SAFE_DELETE(m_pGrapics);
}

HRESULT GameEngine::Init()
{
	m_pGrapics = new Grapics;
	m_pResourceManager = new ResourceManager;

	HRESULT hr = m_pGrapics->Init();
	if (SUCCEEDED(hr))
	{
		hr = m_pResourceManager->Init();
	}

	return hr;
}

void GameEngine::Destroy()
{
	m_pResourceManager->Destroy();
	m_pGrapics->Destroy();
}
