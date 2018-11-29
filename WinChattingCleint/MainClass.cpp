#include "stdafx.h"
#include "MainClass.h"

MainClass::MainClass()
	:isOn(true),
	GameNode(TEXT("MainClass")),
	m_pConfig(nullptr)
{
	m_pConfig = new Config;
	m_pConfig->Init();

	//printf("--ä�� Ŭ���̾�Ʈ--\n");

	//TODO : Ŀ�ǵ� â���� ���� �Է¹ް� ���� �������κ�ȯ 
	//�н����� ó�� ��ȣȭ
	char ServerIp[128] = {0, };
	StrConvT2A(m_pConfig->GetIP(), ServerIp, sizeof(ServerIp));
	u_short	ServerPort = m_pConfig->GetPort();

	//Ŭ���̾�Ʈ ��Ʈ��ũ ���� �� �ʱ�ȭ
	CLIENTNETWORK->Init(ServerIp, ServerPort);
	CLIENTNETWORK->SetLinkIsOn(&isOn);
}

MainClass::~MainClass()
{
	SAFE_DELETE(m_pPlayer);
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
	MakeConsole();

	CLIENTNETWORK->GetServerSession()->GetServerSessionParser()->Init();

	//float x, y = 0.0f;
	//D2D_RENDERTARGET->GetDpi(&x, &y);
	////������ ��Ʈ�ʿ� �׷���
	//DIRECT2D->GetBackBufferTarget()->SetDpi(x, y);

	//�� ����� �ŸŴ����� �־���
	m_pPlayer = new Player(TEXT("Player"));
	m_pPlayer->Init();

	LoginScene* loginScene = new LoginScene(TEXT("LOGIN"), LOGIN);
	loginScene->SetMemoryLinkPlayer(m_pPlayer);
	WaittingChannelScene* waittingChannelScene = new WaittingChannelScene(TEXT("WAITTING_CHANNEL"), WAITTING_CHANNEL);
	waittingChannelScene->SetMemoryLinkPlayer(m_pPlayer);
	InChannelScene* inChannelScene = new InChannelScene(TEXT("IN_CHANNEL"), IN_CHANNEL);
	inChannelScene->SetMemoryLinkPlayer(m_pPlayer);
	SCENEAMANGER->InsertScene(loginScene);
	SCENEAMANGER->InsertScene(waittingChannelScene);
	SCENEAMANGER->InsertScene(inChannelScene);
	SCENEAMANGER->ChangeCurrentScene(LOGIN);

	WSAERROR->Init();
	
	return S_OK;
}

void MainClass::Release()
{
	ReleaseConsole();
}

void MainClass::Update()
{
	SCENEAMANGER->GetCurrentScene()->Update();
}

void MainClass::Render()
{
	ID2D1Bitmap* pBitmap = nullptr;
	DIRECT2D->GetBackBufferTarget()->BeginDraw();
	DIRECT2D->GetBackBufferTarget()->Clear(D2D1::ColorF(D2D1::ColorF::Blue));
	SCENEAMANGER->GetCurrentScene()->Render();
	DIRECT2D->GetBackBufferTarget()->EndDraw();
	DIRECT2D->GetBackBufferTarget()->GetBitmap(&pBitmap);
	
	//����۸� ���� Ÿ�ٿ� �׷���
	D2D1_SIZE_F size = D2D_RENDERTARGET->GetSize();
	D2D1_RECT_F desArea = D2D1::RectF(0.0f, 0.0f, size.width, size.height);
	D2D1_RECT_F sourArea = D2D1::RectF(
		D2D_CAMERA->getCameraLeft(),
		D2D_CAMERA->getCameraTop(),
		D2D_CAMERA->getCameraRight(),
		D2D_CAMERA->getCameraBottom());

	D2D_RENDERTARGET->BeginDraw();
	
	D2D_RENDERTARGET->SetTransform(D2D1::Matrix3x2F::Identity());	//�⺻ ��ķ� ����
	D2D_RENDERTARGET->Clear(D2D1::ColorF(D2D1::ColorF::Blue));	//�޹�� �����
	D2D_RENDERTARGET->DrawBitmap(pBitmap,
		desArea, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		sourArea);

	D2D_RENDERTARGET->EndDraw();

	SAFE_RELEASE(pBitmap);
}

LRESULT MainClass::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_SIZE:
	{
		UINT width = LOWORD(lParam);
		UINT height = HIWORD(lParam);

		D2D1_SIZE_U size = D2D1::SizeU(width, height);
		if (D2D_RENDERTARGET)
		{
			DIRECT2D->ResizeRenderTarget(size);
			DIRECT2D->ResizeBackBufferTarget(size);
			DIRECT2D->ResizeDCRenderTarget(size);
		}
	}
	break;
	}

	if (SCENEAMANGER->GetCurrentScene())
	{
		return SCENEAMANGER->GetCurrentScene()->MainProc(hWnd, iMessage, wParam, lParam);
	}

	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}
