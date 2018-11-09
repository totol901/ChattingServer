#include "stdafx.h"
#include "ServerSessionParser.h"

ServerSessionParser::ServerSessionParser(IOCPServerSession* clientSession)
	:m_ClientSession(clientSession)
{
}

ServerSessionParser::~ServerSessionParser()
{
	m_ClientSession = nullptr;
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
		cout << "로그인 성공" << endl;
		cout << "닉네임 : " << nickname.c_str() << endl << endl;
		
		SCENEAMANGER->ChangeCurrentScene(WAITTING_CHANNEL);
		SCENEAMANGER->GetBeforeScene()->SignalEvent();
		return;
	}

	cout << "로그인 실패 " << endl;
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

	cout << nickname.c_str() << " : " << message.c_str() << endl;

	SCENEAMANGER->ChangeCurrentScene(IN_CHANNEL);
	SCENEAMANGER->GetBeforeScene()->SignalEvent();
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
		cout << "!아이디 없음" << endl;
		break;
	case LOGIN_ERROR_ALREADY_LOGIN:
		cout << "!이미 로그인 중" << endl;
		break;
	case LOGIN_ERROR_CREATEID_ID_ALREADY_EXE:
		cout << "!만드려는 아이디 이미 존재함" << endl;
		break;
	case CREATE_CHANNEL_ALREADY_EXE:
		cout << "!만드려는 채널 이미 존재함" << endl;
		break;
	case ENTER_CHANNEL_CANT_FIND:
		cout << "!들어가려는 채널 없음" << endl;
		break;
	case LEAVE_CHANNEL_CANT_FIND:
		cout << "!나가려는 채널 없음" << endl;
		break;
	
	default:
		cout << "로그인 오류번호 이상" << endl;
		ASSERT(false);
	}
}

