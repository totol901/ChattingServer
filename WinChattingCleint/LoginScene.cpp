#include "stdafx.h"
#include "LoginScene.h"

LoginScene::LoginScene(const WCHAR * nodeName, UINT nodeNum)
	:BaseScene(nodeName, nodeNum),
	m_pLoginUI(nullptr),
	m_pIDTextBoxUI(nullptr),
	m_pPassTextBoxUI(nullptr)
{
}

LoginScene::~LoginScene()
{
	SAFE_DELETE(m_pLoginUI);
}

HRESULT LoginScene::Init()
{
	HRESULT hr = BaseScene::Init();

	//UI 초기화
	//UI 부모 초기화
	m_pLoginUI = new UINode(TEXT("LoginUI"));
	D2D_SIZE_F size = DIRECT2D->GetBackBufferTarget()->GetSize();
	m_pLoginUI->Init(
		D2D_PRIMITEVS->RectFMakeCenter(
			size.width*0.5f,
			size.height*0.5f,
			size.width*0.25f,
			size.height*0.25f)
	);

	//UI 자식 초기화
	m_pLoginUI->AddChildTextBoxUI(
		TEXT("IDTextBox"),
		D2D_PRIMITEVS->RectFMake(
			10,
			10,
			200.0f,
			30.0f),
		15,
		D2D1::ColorF(1.0f, 1.0f, 1.0f)
	);

	m_pLoginUI->AddChildTextBoxUI(
		TEXT("PassTextBox"),
		D2D_PRIMITEVS->RectFMake(
			10,
			50,
			200.0f,
			30.0f),
		15,
		D2D1::ColorF(1.0f, 1.0f, 1.0f)
	);

	m_pIDTextBoxUI = (TextBoxUI*)m_pLoginUI->FindUI(TEXT("IDTextBox"));
	m_pPassTextBoxUI = (TextBoxUI*)m_pLoginUI->FindUI(TEXT("PassTextBox"));

	return hr;
}

void LoginScene::Release()
{
	BaseScene::Release();

	m_pLoginUI->Release();
}

void LoginScene::Update()
{
	BaseScene::Update();

	m_pLoginUI->Update();

	//cout << endl;
	//cout << "------로그인 창-----" << endl;
	//m_SendStream.clear();
	//int selectNum = 0;
	//bool roopOut = false;
	//
	//printf("1. 기존 아이디로 로그인.\n");
	//printf("2. 아이디 생성.\n");
	//printf("3. 종료.\n");
	//
	//cout << "숫자 입력 : ";
	//cin >> selectNum;
	// 
	//switch (selectNum)
	//{
	//	case 1://기존 아이디로 로그인
	//	{
	//		//ID, PW 입력받음
	//		char id[15];
	//		char pw[15];
	//		cout << "ID 입력 : ";
	//		cin >> id;
	//		cout << "PW 입력 : ";
	//		cin >> pw;
	//
	//		//stream에 데이터 넣음
	//		m_SendStream.write(id);
	//		m_SendStream.write(pw);
	//
	//		//입력 받은 데이터 송신
	//		m_Packet.Clear();
	//		m_Packet.type = PK_REQ_LOGIN;
	//		m_Packet.SetStream(m_SendStream);
	//
	//		//패킷 전송
	//		CLIENTNETWORK->GetServerSession()->SendPacket(m_Packet);
	//	}
	//		break;
	//
	//	case 2://아이디 생성
	//	{
	//		//ID, PW, nickname 입력받음
	//		char id[15];
	//		char pw[15];
	//		char nickname[15];
	//		cout << "ID 입력 : ";
	//		cin >> id;
	//		cout << "PW 입력 : ";
	//		cin >> pw;
	//		cout << "nickname 입력 : ";
	//		cin >> nickname;
	//
	//		m_SendStream.write(id);
	//		m_SendStream.write(pw);
	//		m_SendStream.write(nickname);
	//
	//		//입력 받은 데이터 송신
	//		m_Packet.Clear();
	//		m_Packet.type = PK_REQ_CREATE_ID;
	//		m_Packet.SetStream(m_SendStream);
	//
	//		//패킷 전송
	//		CLIENTNETWORK->GetServerSession()->SendPacket(m_Packet);
	//	}
	//		break;
	//
	//	case 3:
	//	{
	//		cout << "종료하기" << endl;
	//		//종료
	//		m_Packet.Clear();
	//		m_Packet.type = PK_REQ_EXIT;
	//		m_Packet.SetStream(m_SendStream);
	//
	//		CLIENTNETWORK->GetServerSession()->SendPacket(m_Packet);
	//		//보내는 스트림 종료
	//		shutdown(CLIENTNETWORK->GetServerSession()->GetSocket(), SD_SEND);
	//	}
	//		break;
	//
	//	default:
	//		cout << "숫자를 다시 입력하시오" << endl << endl;
	//		roopOut = true;
	//		break;
	//}
	//
	//if (CLIENTNETWORK->IsOn())
	//{
	//	WaitForRecvPacket();
	//}
	//
	//cin.ignore();
}

void LoginScene::Render()
{
	BaseScene::Render();

	D2D1_SIZE_F RenderTGsize = D2D_RENDERTARGET->GetSize();

	m_pLoginUI->Render();

	//카메라 세팅
	D2D_CAMERA->setCamera(0.0f, 0.0f, RenderTGsize.width, RenderTGsize.height,
		RenderTGsize.width, RenderTGsize.height);
}

LRESULT LoginScene::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	return BaseScene::MainProc(hWnd, iMessage, wParam, lParam);
}
