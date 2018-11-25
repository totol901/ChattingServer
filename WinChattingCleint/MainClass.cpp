#include "stdafx.h"
#include "MainClass.h"

MainClass::MainClass()
	:isOn(true),
	GameNode(TEXT("MainClass"), 0),
	m_pConfig(nullptr)
{
	MakeConsole();

	m_pConfig = new Config;
	m_pConfig->Init();

	printf("--채팅 클라이언트--\n");

	//TODO : 커맨드 창에서 직접 입력받고 서버 접속으로변환 
	//패스워드 처리 암호화
	char ServerIp[128] = {0, };
	StrConvT2A(m_pConfig->GetIP(), ServerIp, sizeof(ServerIp));
	u_short	ServerPort = m_pConfig->GetPort();

	//클라이언트 네트워크 선언 및 초기화
	CLIENTNETWORK->Init(ServerIp, ServerPort);
	CLIENTNETWORK->SetLinkIsOn(&isOn);

	//신 만들고 신매니저에 넣어줌
	LoginScene* loginScene = new LoginScene(LOGIN);
	WaittingChannelScene* waittingChannelScene = new WaittingChannelScene(WAITTING_CHANNEL);
	InChannelScene* inChannelScene = new InChannelScene(IN_CHANNEL);
	SCENEAMANGER->InsertScene(loginScene);
	SCENEAMANGER->InsertScene(waittingChannelScene);
	SCENEAMANGER->InsertScene(inChannelScene);
	SCENEAMANGER->ChangeCurrentScene(LOGIN);
}

MainClass::~MainClass()
{
	ReleaseConsole();

	SAFE_DELETE(m_pConfig);
}

void MainClass::MakeConsole()
{
#ifdef _DEBUG
	if (AllocConsole())
	{
		FILE* fp = NULL;
		freopen_s(&fp, "CONIN$", "rb", stdin);
		freopen_s(&fp, "CONOUT$", "wb", stdout);
		freopen_s(&fp, "CONOUT$", "wb", stderr);

		std::ios::sync_with_stdio();
	}
#endif
}

void MainClass::ReleaseConsole()
{
#ifdef _DEBUG
	FreeConsole();
#endif
}

HRESULT MainClass::Init()
{
	return S_OK;
}

void MainClass::Release()
{
}

void MainClass::Update()
{
	//신 업데이트
	while (isOn)
	{
		SCENEAMANGER->GetCurrentScene()->Update();
	}
}

void MainClass::Render()
{
}

LRESULT MainClass::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_SIZE:
	{
		UINT width = LOWORD(lParam);
		UINT height = HIWORD(lParam);
		if (D2D_RENDERTARGET)
		{
			DIRECT2D->ResizeRenderTarget(D2D1::SizeU(width, height));
			DIRECT2D->ResizeBackBufferTarget(D2D1::SizeU(width, height));
			DIRECT2D->ResizeDCRenderTarget(D2D1::SizeU(width, height));
		}
	}
	break;
	}

	return GameNode::MainProc(hWnd, iMessage, wParam, lParam);
}
