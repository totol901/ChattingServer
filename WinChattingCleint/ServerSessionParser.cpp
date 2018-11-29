#include "stdafx.h"
#include "ServerSessionParser.h"

ServerSessionParser::ServerSessionParser(IOCPServerSession* clientSession)
	:m_ClientSession(clientSession),
	m_pErrorUI(nullptr),
	m_pStaticTextUI(nullptr)
{
}

ServerSessionParser::~ServerSessionParser()
{
	SAFE_DELETE(m_pErrorUI);
	m_ClientSession = nullptr;
}

void ServerSessionParser::Init()
{
	D2D_SIZE_F size = DIRECT2D->GetBackBufferTarget()->GetSize();
	m_pErrorUI = new UINode(TEXT("ErrorUI"));
	m_pErrorUI->Init(
		D2D_PRIMITEVS->RectFMakeCenter(
			size.width * 0.5f,
			size.height * 0.5f,
			size.width * 0.5f,
			size.height * 0.4f)
	);

	m_pErrorUI->AddChildStaticTextUI(TEXT("ErrorUIText"),
		D2D_PRIMITEVS->RectFMake(
			100,
			10,
			150.0f,
			300.0f),
		TEXT(""),
		D2D1::ColorF(0.0f, 0.3f, 0.0f),
		false
	);

	m_pErrorUI->AddChildButtonUI(TEXT("ErrorUIButton"),
		D2D_PRIMITEVS->RectFMake(
			100,
			50,
			80.0f,
			30.0f),
		std::bind(&ServerSessionParser::ErrorOK, this),
		TEXT("Ok")
	);
	m_pErrorUI->SetOn(true);

	m_pStaticTextUI = (StaticTextUI*)m_pErrorUI->FindUI(TEXT("ErrorUIText"));
}

void ServerSessionParser::ErrorOK()
{
	//�α��� ���� �÷��̾� ������ ����
	LoginScene* pLoginScene = (LoginScene*)SCENEAMANGER->GetCurrentScene();
	pLoginScene->SetState(LOGIN_COMMON);
	m_pErrorUI->SetOn(false);
}

void ServerSessionParser::SetRecvStream(T_PACKET * packet)
{
	recvStream.clear();
	const size_t headSize = sizeof(packet->Size) + sizeof(packet->type);
	recvStream.set(packet->buff, packet->Size - headSize);
}

void ServerSessionParser::AnsLogin(T_PACKET * packet)
{
	bool IsSuccess = false;
	int errorNum = 0;
	string nickname;

	SetRecvStream(packet);
	recvStream.read(&IsSuccess, sizeof(IsSuccess));
	recvStream.read(&errorNum, sizeof(errorNum));
	recvStream.read(nickname);

	//�α��� ���� ����
	if (IsSuccess)
	{
		//cout << "�α��� ����" << endl;
		//cout << "�г��� : " << nickname.c_str() << endl << endl;
		
		//�α��� ���� �÷��̾� ������ ����
		LoginScene* pLoginScene = (LoginScene*)SCENEAMANGER->GetCurrentScene();
		pLoginScene->GetPlayer()->SetPlayerNickname(nickname);
		pLoginScene->GetPlayer()->SetPlayerState(PLAYER_LOGIN);
		pLoginScene->SetState(LOGIN_COMMON);

		//������ ä�� ������ �ٲ���
		SCENEAMANGER->ChangeCurrentScene(WAITTING_CHANNEL);
		SCENEAMANGER->GetBeforeScene()->SignalEvent();
		return;
	}

	LoginScene* pLoginScene = (LoginScene*)SCENEAMANGER->GetCurrentScene();
	//cout << "�α��� ���� " << endl;
	//cout << "���� ��ȣ :" << errorNum << endl;
	ErrorPrint(errorNum);

	SCENEAMANGER->ChangeCurrentScene(LOGIN);
	SCENEAMANGER->GetBeforeScene()->SignalEvent();
}

void ServerSessionParser::AnsCreateId(T_PACKET * packet)
{
	bool IsSuccess = false;
	int errorNum = 0;

	SetRecvStream(packet);
	recvStream.read(&IsSuccess, sizeof(IsSuccess));
	recvStream.read(&errorNum, sizeof(errorNum));

	if (IsSuccess)
	{
		cout << "���̵� ���� ����" << endl;

		SCENEAMANGER->ChangeCurrentScene(LOGIN);
		SCENEAMANGER->GetBeforeScene()->SignalEvent();
		return;
	}

	cout << "���̵� ���� ���� " << endl;
	ErrorPrint(errorNum);

	SCENEAMANGER->ChangeCurrentScene(LOGIN);
	//SCENEAMANGER->GetCurrentScene()->
	SCENEAMANGER->GetBeforeScene()->SignalEvent();
}

void ServerSessionParser::AnsWaitingChannalEnter(T_PACKET * packet)
{
	bool IsSuccess = false;
	int errornum = 0;
	string channelList;

	SetRecvStream(packet);
	recvStream.read(&IsSuccess, sizeof(IsSuccess));
	recvStream.read(&errornum, sizeof(errornum));
	recvStream.read(channelList);

	cout << "ä�� ����Ʈ" << endl;
	if (IsSuccess)
	{
		if (channelList[0] != '\0')
		{
			cout << channelList.c_str();
		}
		else
		{
			cout << "������ ä���� ����." << endl;
		}
	}

	SCENEAMANGER->ChangeCurrentScene(WAITTING_CHANNEL);
	SCENEAMANGER->GetBeforeScene()->SignalEvent();
}

void ServerSessionParser::AnsWaitingChannelCreateChannel(T_PACKET * packet)
{
	bool IsSuccess = false;
	int errornum = 0;

	SetRecvStream(packet);
	recvStream.read(&IsSuccess, sizeof(IsSuccess));
	recvStream.read(&errornum, sizeof(errornum));

	if (IsSuccess)
	{
		cout << "ä�� ���� ����" << endl;

		SCENEAMANGER->ChangeCurrentScene(IN_CHANNEL);
		SCENEAMANGER->GetBeforeScene()->SignalEvent();
		return;
	}

	cout << "ä�� ���� ���� " << endl;
	ErrorPrint(errornum);

	SCENEAMANGER->ChangeCurrentScene(WAITTING_CHANNEL);
	SCENEAMANGER->GetBeforeScene()->SignalEvent();
}

void ServerSessionParser::AnsWaitingChannelJoin(T_PACKET * packet)
{
	bool IsSuccess = false;
	int errornum = 0;

	SetRecvStream(packet);
	recvStream.read(&IsSuccess, sizeof(IsSuccess));
	recvStream.read(&errornum, sizeof(errornum));

	if (IsSuccess)
	{
		cout << "ä�� ���� ����" << endl;

		SCENEAMANGER->ChangeCurrentScene(IN_CHANNEL);
		SCENEAMANGER->GetBeforeScene()->SignalEvent();
		return;
	}

	cout << "ä�� ���� ���� " << endl;
	ErrorPrint(errornum);

	SCENEAMANGER->ChangeCurrentScene(WAITTING_CHANNEL);
	SCENEAMANGER->GetBeforeScene()->SignalEvent();
}

void ServerSessionParser::RecvChannelMessage(T_PACKET * packet)
{
	string nickname;
	string message;

	SetRecvStream(packet);
	recvStream.read(nickname);
	recvStream.read(message);

	cout << endl << nickname.c_str() << " : " << message.c_str() << endl;

	//SCENEAMANGER->ChangeCurrentScene(IN_CHANNEL);
	//SCENEAMANGER->GetBeforeScene()->SignalEvent();
}

void ServerSessionParser::AnsChannelOut(T_PACKET * packet)
{
	bool IsSuccess = false;
	int errornum = 0;

	SetRecvStream(packet);
	recvStream.read(&IsSuccess, sizeof(IsSuccess));
	recvStream.read(&errornum, sizeof(errornum));
	
	if (IsSuccess)
	{
		cout << "ä�� ������ ����" << endl;


		SCENEAMANGER->ChangeCurrentScene(WAITTING_CHANNEL);
		SCENEAMANGER->GetBeforeScene()->SignalEvent();

		return;
	}

	cout << "ä�� ������ ���� " << endl;
	ErrorPrint(errornum);

	SCENEAMANGER->ChangeCurrentScene(IN_CHANNEL);
	SCENEAMANGER->GetBeforeScene()->SignalEvent();
}

void ServerSessionParser::AnsExit(T_PACKET * packet)
{
	bool IsSuccess = false;
	int errornum = 0;

	SetRecvStream(packet);
	recvStream.read(&IsSuccess, sizeof(IsSuccess));
	recvStream.read(&errornum, sizeof(errornum));

	if (IsSuccess)
	{
		cout << "���� ���� ���� ����" << endl;
		m_ClientSession->Disconnect();
		SCENEAMANGER->GetCurrentScene()->SignalEvent();
		return;
	}

	cout << "���� ���� ���� ���� " << endl;
	cout << "���� ��ȣ :" << errornum << endl;
	SCENEAMANGER->GetCurrentScene()->SignalEvent();

	//�޴� ��Ʈ�� ����
	shutdown(m_ClientSession->GetSocket(), SD_RECEIVE);
}

bool ServerSessionParser::PacketParsing(T_PACKET * const packet)
{
	recvStream.clear();
	SendStream.clear();
	
	switch (packet->type)
	{
	case PK_NONE:
		cout << "test : " << packet->buff << endl;
		break;

	case PK_ANS_LOGIN:
		AnsLogin(packet);
		break;

	case PK_ANS_CREATE_ID:
		AnsCreateId(packet);
		break;

	case PK_ANS_WAITINGCHANNAL_ENTER:
		AnsWaitingChannalEnter(packet);
		break;

	case PK_ANS_WAITINGCHANNAL_CHREAT_CHANNAL:
		AnsWaitingChannelCreateChannel(packet);
		break;

	case PK_ANS_WAITINGCHANNAL_CHANNAL_JOIN:
		AnsWaitingChannelJoin(packet);
		break;

	case PK_RECV_CHANNAL_MESSAGE:
		RecvChannelMessage(packet);
		break;

	case PK_ANS_CHANNAL_OUT:
		AnsChannelOut(packet);
		break;

	case PK_ANS_EXIT:
		AnsExit(packet);
		return false;

	default:
		ASSERT(false);
	}

	return true;
}

void ServerSessionParser::ErrorPrint(UINT errorNum)
{
	switch (errorNum)
	{
	case LOGIN_ERROR_WRONG:
	{
		m_pStaticTextUI->SetTextStr(TEXT("!���̵�, ��й�ȣ ����"));
		LoginScene* pLoginScene = (LoginScene*)SCENEAMANGER->GetCurrentScene();
		m_pErrorUI->SetOn(true);
		WSAERROR->SetPopupErrorUI(m_pErrorUI);
		
		//cout << "!���̵� ����" << endl;
	}
		break;
	case LOGIN_ERROR_ALREADY_LOGIN:
	{
		m_pStaticTextUI->SetTextStr(TEXT("!�̹� �α��� ��"));
		LoginScene* pLoginScene = (LoginScene*)SCENEAMANGER->GetCurrentScene();
		m_pErrorUI->SetOn(true);
		WSAERROR->SetPopupErrorUI(m_pErrorUI);
		//cout << "!�̹� �α��� ��" << endl;
	}
		break;
	case LOGIN_ERROR_CREATEID_ID_ALREADY_EXE:
	{
		m_pStaticTextUI->SetTextStr(TEXT("!������� ���̵� �̹� ������"));
		LoginScene* pLoginScene = (LoginScene*)SCENEAMANGER->GetCurrentScene();
		m_pErrorUI->SetOn(true);
		WSAERROR->SetPopupErrorUI(m_pErrorUI);
		//cout << "!������� ���̵� �̹� ������" << endl;
	}
		break;
	case CREATE_CHANNEL_ALREADY_EXE:
		//m_pStaticTextUI->SetTextStr(TEXT("!������� ä�� �̹� ������"));
		//LoginScene* pLoginScene = (LoginScene*)SCENEAMANGER->GetCurrentScene();
		//pLoginScene->SetPopupErrorUI(m_pErrorUI);
		//cout << "!������� ä�� �̹� ������" << endl;
		break;
	case ENTER_CHANNEL_CANT_FIND:
		//m_pStaticTextUI->SetTextStr(TEXT("!������ ä�� ����"));
		//LoginScene* pLoginScene = (LoginScene*)SCENEAMANGER->GetCurrentScene();
		//pLoginScene->SetPopupErrorUI(m_pErrorUI);
		//cout << "!������ ä�� ����" << endl;
		break;
	case LEAVE_CHANNEL_CANT_FIND:
		//m_pStaticTextUI->SetTextStr(TEXT("!�������� ä�� ����"));
		//LoginScene* pLoginScene = (LoginScene*)SCENEAMANGER->GetCurrentScene();
		//pLoginScene->SetPopupErrorUI(m_pErrorUI);
		//cout << "!�������� ä�� ����" << endl;
		break;
	default:
		m_pStaticTextUI->SetTextStr(TEXT("�α��� ������ȣ �̻�"));
		LoginScene* pLoginScene = (LoginScene*)SCENEAMANGER->GetCurrentScene();
		m_pErrorUI->SetOn(true);
		WSAERROR->SetPopupErrorUI(m_pErrorUI);
		//cout << "�α��� ������ȣ �̻�" << endl;
		ASSERT(false);
	}
}
