#include "stdafx.h"
#include "LoginScene.h"

LoginScene::LoginScene(const WCHAR * nodeName, UINT nodeNum)
	:BaseScene(nodeName, nodeNum),
	m_pLoginUI(nullptr),
	m_pIDTextBoxUI(nullptr),
	m_pPassTextBoxUI(nullptr),
	m_eLoginState(LOGIN_COMMON)
{
}

LoginScene::~LoginScene()
{
	SAFE_DELETE(m_pLoginUI);
}

HRESULT LoginScene::InitLoginUI()
{
	//UI �θ� �ʱ�ȭ
	m_pLoginUI = new UINode(TEXT("LoginUI"));
	D2D_SIZE_F size = DIRECT2D->GetBackBufferTarget()->GetSize();
	HRESULT hr = m_pLoginUI->Init(
		D2D_PRIMITEVS->RectFMakeCenter(
			size.width*0.5f,
			size.height*0.5f,
			size.width*0.5f,
			size.height*0.5f)
	);

	//UI �ڽ� �ʱ�ȭ
	m_pLoginUI->AddChildTextBoxUI(
		TEXT("IDTextBox"),
		D2D_PRIMITEVS->RectFMake(
			100,
			10,
			150.0f,
			30.0f),
		15,
		D2D1::ColorF(1.0f, 1.0f, 1.0f)
	);

	m_pLoginUI->AddChildStaticTextUI(
		TEXT("ID"),
		D2D_PRIMITEVS->RectFMake(
			70,
			10,
			20.0f,
			30.0f),
		TEXT("ID"),
		D2D1::ColorF(1.0f, 1.0f, 1.0f),
		false
	);

	m_pLoginUI->AddChildTextBoxUI(
		TEXT("PassTextBox"),
		D2D_PRIMITEVS->RectFMake(
			100,
			50,
			150.0f,
			30.0f),
		15,
		D2D1::ColorF(1.0f, 1.0f, 1.0f)
	);

	m_pLoginUI->AddChildStaticTextUI(
		TEXT("PW"),
		D2D_PRIMITEVS->RectFMake(
			70,
			50,
			25.0f,
			30.0f),
		TEXT("PW"),
		D2D1::ColorF(1.0f, 1.0f, 1.0f),
		false
	);

	m_pLoginUI->AddChildButtonUI(
		TEXT("LoginButton"),
		D2D_PRIMITEVS->RectFMake(
			100,
			100,
			100.0f,
			30.0f),
		std::bind(&LoginScene::SendLogin, this),
		TEXT("Login"),
		D2D_PRIMITEVS->RectFMake(
			0,
			0,
			100.0f,
			30.0f)
	);

	m_pLoginUI->AddChildButtonUI(
		TEXT("LoginButton"),
		D2D_PRIMITEVS->RectFMake(
			100,
			150,
			100.0f,
			30.0f),
		std::bind(&LoginScene::SendCreateID, this),
		TEXT("Create ID"),
		D2D_PRIMITEVS->RectFMake(
			0,
			0,
			100.0f,
			30.0f)
	);

	m_pLoginUI->SetOn(true);

	//UINode���� FindUI �ż���� TextBox�� ����� �� ������,
	//�ӵ� ���鿡�� �ٶ��� ���� �ʴ� �����Ͽ� TextBox�� �����ͷ� ���� �޾���
	m_pIDTextBoxUI = (TextBoxUI*)m_pLoginUI->FindUI(TEXT("IDTextBox"));
	m_pPassTextBoxUI = (TextBoxUI*)m_pLoginUI->FindUI(TEXT("PassTextBox"));
	m_pPassTextBoxUI->SetCharToStar(true);

	return hr;
}

void LoginScene::SendLogin()
{
	char id[15] = {0, };
	char pw[15] = {0, };

	m_SendStream.clear();
	
	StrConvT2A(m_pIDTextBoxUI->GetText(), id, sizeof(id));
	StrConvT2A(m_pPassTextBoxUI->GetText(), pw, sizeof(pw));

	//stream�� ������ ����
	m_SendStream.write(id);
	m_SendStream.write(pw);
	
	//�Է� ���� ������ �۽�
	m_Packet.Clear();
	m_Packet.type = PK_REQ_LOGIN;
	m_Packet.SetStream(m_SendStream);
	
	//��Ŷ ����
	CLIENTNETWORK->GetServerSession()->SendPacket(m_Packet);

	//recv��Ŷ ������ ���� ����
	if (CLIENTNETWORK->IsOn())
	{
		if (m_eLoginState == LOGIN_COMMON)
		{
			m_eLoginState = LOGIN_WAIT;
			_beginthreadex(
				NULL,
				0,
				LoginRecvThread,
				&m_eLoginState, 0, NULL
			);
		}
	}
}

void LoginScene::SendCreateID()
{
	//ID, PW, nickname �Է¹���
	char id[15];
	char pw[15];
	char nickname[15];
	
	StrConvT2A(m_pIDTextBoxUI->GetText(), id, sizeof(id));
	StrConvT2A(m_pPassTextBoxUI->GetText(), pw, sizeof(pw));
	StrConvT2A(m_pIDTextBoxUI->GetText(), nickname, sizeof(nickname));
	
	m_SendStream.write(id);
	m_SendStream.write(pw);
	m_SendStream.write(nickname);
	
	//�Է� ���� ������ �۽�
	m_Packet.Clear();
	m_Packet.type = PK_REQ_CREATE_ID;
	m_Packet.SetStream(m_SendStream);
	
	//��Ŷ ����
	CLIENTNETWORK->GetServerSession()->SendPacket(m_Packet);

	//recv��Ŷ ������ ���� ����
	if (CLIENTNETWORK->IsOn())
	{
		if (m_eLoginState == LOGIN_COMMON)
		{
			m_eLoginState = LOGIN_WAIT;
			_beginthreadex(
				NULL,
				0,
				LoginRecvThread,
				&m_eLoginState, 0, NULL
			);
		}
	}
}

unsigned int __stdcall LoginScene::LoginRecvThread(LPVOID param)
{
	eLoginSceneState* ploginScene = (eLoginSceneState*)param;

	while (1)
	{
		switch (WaitForRecvPacket())
		{
			//�α��� ó�� �ð� �ʰ� ����
		case WAIT_TIMEOUT:
			
			WSAERROR->PopupWaitingUIOnOff(true);
			*ploginScene = LOGIN_WAIT;
			break;

			//�ñ׳� ��
		case WAIT_OBJECT_0:
			WSAERROR->PopupWaitingUIOnOff(false);
			return 0;

		default:
			Assert(0);
			return 0;
		}
	}

	return 0;
}

HRESULT LoginScene::Init()
{
	HRESULT hr = BaseScene::Init();

	//UI �ʱ�ȭ
	if (SUCCEEDED(hr))
	{
		hr = InitLoginUI();
	}

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
	
	switch (m_eLoginState)
	{
	case LOGIN_COMMON:
		{
			m_pLoginUI->Update();
		}
		break;
	}

	WSAERROR->ErrUpdate();
	
	//cout << endl;
	//cout << "------�α��� â-----" << endl;
	//m_SendStream.clear();
	//int selectNum = 0;
	//bool roopOut = false;
	//
	//printf("1. ���� ���̵�� �α���.\n");
	//printf("2. ���̵� ����.\n");
	//printf("3. ����.\n");
	//
	//cout << "���� �Է� : ";
	//cin >> selectNum;
	// 
	//switch (selectNum)
	//{
	//	case 1://���� ���̵�� �α���
	//	{
	//		//ID, PW �Է¹���
	//		char id[15];
	//		char pw[15];
	//		cout << "ID �Է� : ";
	//		cin >> id;
	//		cout << "PW �Է� : ";
	//		cin >> pw;
	//
	//		//stream�� ������ ����
	//		m_SendStream.write(id);
	//		m_SendStream.write(pw);
	//
	//		//�Է� ���� ������ �۽�
	//		m_Packet.Clear();
	//		m_Packet.type = PK_REQ_LOGIN;
	//		m_Packet.SetStream(m_SendStream);
	//
	//		//��Ŷ ����
	//		CLIENTNETWORK->GetServerSession()->SendPacket(m_Packet);
	//	}
	//		break;
	//
	//	case 2://���̵� ����
	//	{
	//		//ID, PW, nickname �Է¹���
	//		char id[15];
	//		char pw[15];
	//		char nickname[15];
	//		cout << "ID �Է� : ";
	//		cin >> id;
	//		cout << "PW �Է� : ";
	//		cin >> pw;
	//		cout << "nickname �Է� : ";
	//		cin >> nickname;
	//
	//		m_SendStream.write(id);
	//		m_SendStream.write(pw);
	//		m_SendStream.write(nickname);
	//
	//		//�Է� ���� ������ �۽�
	//		m_Packet.Clear();
	//		m_Packet.type = PK_REQ_CREATE_ID;
	//		m_Packet.SetStream(m_SendStream);
	//
	//		//��Ŷ ����
	//		CLIENTNETWORK->GetServerSession()->SendPacket(m_Packet);
	//	}
	//		break;
	//
	//	case 3:
	//	{
	//		cout << "�����ϱ�" << endl;
	//		//����
	//		m_Packet.Clear();
	//		m_Packet.type = PK_REQ_EXIT;
	//		m_Packet.SetStream(m_SendStream);
	//
	//		CLIENTNETWORK->GetServerSession()->SendPacket(m_Packet);
	//		//������ ��Ʈ�� ����
	//		shutdown(CLIENTNETWORK->GetServerSession()->GetSocket(), SD_SEND);
	//	}
	//		break;
	//
	//	default:
	//		cout << "���ڸ� �ٽ� �Է��Ͻÿ�" << endl << endl;
	//		roopOut = true;
	//		break;
	//}
	//
	
	//cin.ignore();
}

void LoginScene::Render()
{
	BaseScene::Render();

	D2D1_SIZE_F RenderTGsize = DIRECT2D->GetBackBufferTarget()->GetSize();

	m_pLoginUI->Render();

	WSAERROR->ErrRender();

	//ī�޶� ����
	D2D_CAMERA->setCamera(0.0f, 0.0f, RenderTGsize.width, RenderTGsize.height,
		RenderTGsize.width, RenderTGsize.height);
}

LRESULT LoginScene::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	if (m_pIDTextBoxUI)
	{
		m_pIDTextBoxUI->MainProc(hWnd, iMessage, wParam, lParam);
	}
	if (m_pPassTextBoxUI)
	{
		m_pPassTextBoxUI->MainProc(hWnd, iMessage, wParam, lParam);
	}

	return BaseScene::MainProc(hWnd, iMessage, wParam, lParam);
}
