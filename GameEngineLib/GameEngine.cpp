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

HRESULT GameEngine::Init(bool useLFH, int winWidth, int winHeight)
{
	if (useLFH)
	{
		UseLFH();
	}

	m_pGrapics = new Grapics;
	m_pResourceManager = new ResourceManager;

	HRESULT hr = m_pGrapics->Init();
	if (SUCCEEDED(hr))
	{
		hr = m_pResourceManager->Init();
	}

	SetWinDIP(winWidth, winHeight);

	return hr;
}

void GameEngine::Destroy()
{
	m_pResourceManager->Destroy();
	m_pGrapics->Destroy();
}

void GameEngine::UseLFH()
{
	//힙 메모리 세팅 사용
	HANDLE Heaps[1024];
	DWORD Count = GetProcessHeaps(1024, Heaps);
	for (DWORD i = 0; i < Count; ++i)
	{
		ULONG  HeapFragValue = 2;

		if (!HeapSetInformation(Heaps[i], HeapCompatibilityInformation, &HeapFragValue, sizeof(HeapFragValue)))
		{
#ifdef _DEBUG
			err_display(TEXT("HeapSetInformation"));
#endif
		}
	}
}

void GameEngine::SetWinDIP(int winWidth, int winHeight)
{
	//윈도우 dpi 받아온다
	FLOAT dpiX, dpiY;
	D2D_FACTORY->GetDesktopDpi(&dpiX, &dpiY);
	//윈도우 창 크기 정해줌
	RECT rc;
	GetClientRect(NULL, &rc);

	RECT winRect;

	winRect.left = 0;
	winRect.top = 0;
	winRect.right = winWidth;
	winRect.bottom = winHeight;

	//비작업 영역을 제외한 작업 영역안의 크기만 맞춰줌.
	AdjustWindowRect(&winRect, CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, false);

	SetWindowPos(g_hWnd, NULL, rc.left, rc.right, (winRect.right - winRect.left),
		(winRect.bottom - winRect.top), SWP_NOZORDER | SWP_NOMOVE);
}
