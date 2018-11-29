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
	//로그인 신의 플레이어 데이터 갱신
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

	//로그인 성공 유무
	if (IsSuccess)
	{
		//cout << "로그인 성공" << endl;
		//cout << "닉네임 : " << nickname.c_str() << endl << endl;
		
		//로그인 신의 플레이어 데이터 갱신
		LoginScene* pLoginScene = (LoginScene*)SCENEAMANGER->GetCurrentScene();
		pLoginScene->GetPlayer()->SetPlayerNickname(nickname);
		pLoginScene->GetPlayer()->SetPlayerState(PLAYER_LOGIN);
		pLoginScene->SetState(LOGIN_COMMON);

		//웨이팅 채널 신으로 바꿔줌
		SCENEAMANGER->ChangeCurrentScene(WAITTING_CHANNEL);
		SCENEAMANGER->GetBeforeScene()->SignalEvent();
		return;
	}

	LoginScene* pLoginScene = (LoginScene*)SCENEAMANGER->GetCurrentScene();
	//cout << "로그인 실패 " << endl;
	//cout << "오류 번호 :" << errorNum << endl;
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
		cout << "아이디 생성 성공" << endl;

		SCENEAMANGER->ChangeCurrentScene(LOGIN);
		SCENEAMANGER->GetBeforeScene()->SignalEvent();
		return;
	}

	cout << "아이디 생성 실패 " << endl;
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

	cout << "채널 리스트" << endl;
	if (IsSuccess)
	{
		if (channelList[0] != '\0')
		{
			cout << channelList.c_str();
		}
		else
		{
			cout << "생성된 채널이 없음." << endl;
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
		cout << "채널 생성 성공" << endl;

		SCENEAMANGER->ChangeCurrentScene(IN_CHANNEL);
		SCENEAMANGER->GetBeforeScene()->SignalEvent();
		return;
	}

	cout << "채널 생성 실패 " << endl;
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
		cout << "채널 입장 성공" << endl;

		SCENEAMANGER->ChangeCurrentScene(IN_CHANNEL);
		SCENEAMANGER->GetBeforeScene()->SignalEvent();
		return;
	}

	cout << "채널 입장 실패 " << endl;
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
		cout << "채널 나가기 성공" << endl;


		SCENEAMANGER->ChangeCurrentScene(WAITTING_CHANNEL);
		SCENEAMANGER->GetBeforeScene()->SignalEvent();

		return;
	}

	cout << "채널 나가기 실패 " << endl;
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
		cout << "서버 연결 종료 성공" << endl;
		m_ClientSession->Disconnect();
		SCENEAMANGER->GetCurrentScene()->SignalEvent();
		return;
	}

	cout << "서버 연결 종료 실패 " << endl;
	cout << "오류 번호 :" << errornum << endl;
	SCENEAMANGER->GetCurrentScene()->SignalEvent();

	//받는 스트림 종료
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
		m_pStaticTextUI->SetTextStr(TEXT("!아이디, 비밀번호 오류"));
		LoginScene* pLoginScene = (LoginScene*)SCENEAMANGER->GetCurrentScene();
		m_pErrorUI->SetOn(true);
		WSAERROR->SetPopupErrorUI(m_pErrorUI);
		
		//cout << "!아이디 없음" << endl;
	}
		break;
	case LOGIN_ERROR_ALREADY_LOGIN:
	{
		m_pStaticTextUI->SetTextStr(TEXT("!이미 로그인 중"));
		LoginScene* pLoginScene = (LoginScene*)SCENEAMANGER->GetCurrentScene();
		m_pErrorUI->SetOn(true);
		WSAERROR->SetPopupErrorUI(m_pErrorUI);
		//cout << "!이미 로그인 중" << endl;
	}
		break;
	case LOGIN_ERROR_CREATEID_ID_ALREADY_EXE:
	{
		m_pStaticTextUI->SetTextStr(TEXT("!만드려는 아이디 이미 존재함"));
		LoginScene* pLoginScene = (LoginScene*)SCENEAMANGER->GetCurrentScene();
		m_pErrorUI->SetOn(true);
		WSAERROR->SetPopupErrorUI(m_pErrorUI);
		//cout << "!만드려는 아이디 이미 존재함" << endl;
	}
		break;
	case CREATE_CHANNEL_ALREADY_EXE:
		//m_pStaticTextUI->SetTextStr(TEXT("!만드려는 채널 이미 존재함"));
		//LoginScene* pLoginScene = (LoginScene*)SCENEAMANGER->GetCurrentScene();
		//pLoginScene->SetPopupErrorUI(m_pErrorUI);
		//cout << "!만드려는 채널 이미 존재함" << endl;
		break;
	case ENTER_CHANNEL_CANT_FIND:
		//m_pStaticTextUI->SetTextStr(TEXT("!들어가려는 채널 없음"));
		//LoginScene* pLoginScene = (LoginScene*)SCENEAMANGER->GetCurrentScene();
		//pLoginScene->SetPopupErrorUI(m_pErrorUI);
		//cout << "!들어가려는 채널 없음" << endl;
		break;
	case LEAVE_CHANNEL_CANT_FIND:
		//m_pStaticTextUI->SetTextStr(TEXT("!나가려는 채널 없음"));
		//LoginScene* pLoginScene = (LoginScene*)SCENEAMANGER->GetCurrentScene();
		//pLoginScene->SetPopupErrorUI(m_pErrorUI);
		//cout << "!나가려는 채널 없음" << endl;
		break;
	default:
		m_pStaticTextUI->SetTextStr(TEXT("로그인 오류번호 이상"));
		LoginScene* pLoginScene = (LoginScene*)SCENEAMANGER->GetCurrentScene();
		m_pErrorUI->SetOn(true);
		WSAERROR->SetPopupErrorUI(m_pErrorUI);
		//cout << "로그인 오류번호 이상" << endl;
		ASSERT(false);
	}
}
