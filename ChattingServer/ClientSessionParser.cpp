#include "stdafx.h"
#include "ClientSessionParser.h"

static CRITICAL_SECTION ChannelInOutCS;

ClientSessionParser::ClientSessionParser(ClientSession * clientSession)
	:m_ClientSession(clientSession)
{
	InitializeCriticalSection(&ChannelInOutCS);
}

ClientSessionParser::~ClientSessionParser()
{
	DeleteCriticalSection(&ChannelInOutCS);
	m_ClientSession = nullptr;
}

void ClientSessionParser::SendPacketWithSendStream(E_PACKET_TYPE && type)
{
	m_SendPk.Clear();
	m_SendPk.type = type;
	m_SendPk.SetStream(SendStream);

	m_ClientSession->SendPacket(m_SendPk);
}

void ClientSessionParser::SetRecvStream(T_PACKET * packet)
{
	recvStream.clear();
	const size_t headSize = sizeof(packet->Size) + sizeof(packet->type);
	recvStream.set(packet->buff, packet->Size - headSize);
}

void ClientSessionParser::ReqLogin(T_PACKET * packet)
{
	recvStream.clear();
	SendStream.clear();
	string id;
	string pw;
	bool isSuccess = false;
	int errorNum = 0;

	//stream에 받은 데이터 넣어줌
	const size_t headSize = sizeof(packet->Size) + sizeof(packet->type);
	recvStream.set(packet->buff, packet->Size - headSize);
	recvStream.read(id);
	recvStream.read(pw);

	//id, pw 쿼리 채크
	if (DATABASE->CheckUserInfoQuery(id, pw))
	{
		//로그인 중인지 확인
		if (CLIENTSESSIONMANAGER->FindClientSessionID(
			id) != nullptr)
		{
			//로그인 이미 함
			isSuccess = false;
			errorNum = LOGIN_ERROR_ALREADY_LOGIN;

			//stream에 보내줄 data 써줌
			SendStream.write(&isSuccess, sizeof(isSuccess));
			SendStream.write(&errorNum, sizeof(errorNum));

			SLogPrint("%s - 로그인 실패", id.c_str());

			//데이터베이스에 로그 남겨줌
			DATABASE->InsertUserLogQuery(id, "로그인 실패");
		}
		else
		{
			//로그인 성공
			isSuccess = true;
			errorNum = ERROR_NONE;

			//stream에 보내줄 data 써줌
			string nickname = DATABASE->FindNickname(id);
			SendStream.write(&isSuccess, sizeof(isSuccess));
			SendStream.write(&errorNum, sizeof(errorNum));
			SendStream.write(nickname);

			SLogPrint("%s - 로그인 성공", id.c_str());

			//플레이어 데이터에 아이디 닉네임 저장
			m_ClientSession->GetPlayerData()->SetPlayerID(id);
			m_ClientSession->GetPlayerData()->SetPlayerNickname(nickname);
			m_ClientSession->GetPlayerData()->SetPlayerState(PLAYER_LOGIN);

			//데이터베이스에 로그 남겨줌
			DATABASE->InsertUserLogQuery(id, "로그인 완료");

			//로그인된 세션 풀에 넣음
			CLIENTSESSIONMANAGER->AddClientSessionID(id, m_ClientSession);
		}
	}
	else
	{
		isSuccess = false;
		errorNum = LOGIN_ERROR_WRONG;

		//stream에 보내줄 data 써줌
		SendStream.write(&isSuccess, sizeof(isSuccess));
		SendStream.write(&errorNum, sizeof(errorNum));

		SLogPrint("%s - 로그인 실패", id);

		//데이터베이스에 로그 남겨줌
		DATABASE->InsertUserLogQuery(id, "로그인 실패");
	}

	//패킷 송신
	m_SendPk.Clear();
	m_SendPk.type = PK_ANS_LOGIN;
	m_SendPk.SetStream(SendStream);
	m_ClientSession->SendPacket(m_SendPk);
}

void ClientSessionParser::ReqCreateID(T_PACKET * packet)
{
	recvStream.clear();
	SendStream.clear();
	string id;
	string pw;
	string nickname;
	bool isSucces = false;
	int errNum = 0;

	const size_t headSize = sizeof(packet->Size) + sizeof(packet->type);
	recvStream.set(packet->buff, packet->Size - headSize);
	recvStream.read(id);
	recvStream.read(pw);
	recvStream.read(nickname);

	if (DATABASE->InsertUserInfoQuery(id, pw, nickname))
	{
		//아이디 생성 성공
		isSucces = true;
		errNum = ERROR_NONE;

		SendStream.write(&isSucces, sizeof(isSucces));
		SendStream.write(&errNum, sizeof(errNum));

		//데이터베이스에 로그 남겨줌
		DATABASE->InsertUserLogQuery(id, "아이디 생성");
	}
	else
	{
		isSucces = false;
		errNum = LOGIN_ERROR_CREATEID_ID_ALREADY_EXE;

		SendStream.write(&isSucces, sizeof(isSucces));
		SendStream.write(&errNum, sizeof(errNum));

		SLogPrint("%s - 아이디 생성 실패", id);
	}

	m_SendPk.Clear();
	m_SendPk.type = PK_ANS_CREATE_ID;
	m_SendPk.SetStream(SendStream);
	m_ClientSession->SendPacket(m_SendPk);
}

void ClientSessionParser::ReqWatingChannelEnter(T_PACKET * packet)
{
	SendStream.clear();
	bool isSucces = true;
	int errNum = ERROR_NONE;
	string vecChannel = CHANNELMANAGER->GetChannelList();

	if (vecChannel.length() > PAKCET_BUFF_SIZE)
	{
		char tempStr[PAKCET_BUFF_SIZE];
		for (int i = 0; i < PAKCET_BUFF_SIZE; i++)
		{
			tempStr[i] = vecChannel[i];
		}
		vecChannel = tempStr;
	}

	SendStream.write(&isSucces, sizeof(isSucces));
	SendStream.write(&errNum, sizeof(errNum));
	SendStream.write(vecChannel);

	//플레이어 데이터에 상태 업뎃
	m_ClientSession->GetPlayerData()->SetPlayerState(PLAYER_WAITING_CHANNEL);

	m_SendPk.Clear();
	m_SendPk.type = PK_ANS_WAITINGCHANNAL_ENTER;
	m_SendPk.SetStream(SendStream);
	m_ClientSession->SendPacket(m_SendPk);
}

void ClientSessionParser::ReqWaitingChannelCreateChannel(T_PACKET * packet)
{
	SendStream.clear();
	//채널 생성
	string channelName;
	SetRecvStream(packet);
	recvStream.read(channelName);

	if (CHANNELMANAGER->MakeChannelWithChannelName(channelName))
	{
		SLogPrintAtFile("%s : 채널 생성 성공", m_ClientSession->GetPlayerData()->GetPlayerNickname().c_str());

		//생성 완료됬다는 패킷 전송
		bool isSucces = true;
		int errNum = ERROR_NONE;

		SendStream.write(&isSucces, sizeof(isSucces));
		SendStream.write(&errNum, sizeof(errNum));
		SendPacketWithSendStream(PK_ANS_WAITINGCHANNAL_CHREAT_CHANNAL);

		//생성한 채널에 입장
		if (CHANNELMANAGER->ClientJoinTheChannel(m_ClientSession, channelName))
		{
			SLogPrint("채널 입장 성공");
		}
		else
		{
			SLogPrintAtFile("!채널 생성후 입장 실패");
			ASSERT(FALSE);
		}

		return;
	}
	
	//생성 실패
	bool isSucces = false;
	int errNum = CREATE_CHANNEL_ALREADY_EXE;

	SendStream.write(&isSucces, sizeof(isSucces));
	SendStream.write(&errNum, sizeof(errNum));
	SendPacketWithSendStream(PK_ANS_WAITINGCHANNAL_CHREAT_CHANNAL);
}

void ClientSessionParser::ReqWaitingChannelChannelJoin(T_PACKET * packet)
{
	SendStream.clear();
	//해당 채널 찾음
	UINT channelNum = 0;
	SetRecvStream(packet);
	recvStream.read(&channelNum, sizeof(channelNum));

	//채널 찾음 유무에 따라 패킷 송신
	EnterCriticalSection(&ChannelInOutCS);
	Channel* channel = CHANNELMANAGER->FindChannelByID(channelNum);
	if (channel)
	{
		bool isSucces = true;
		int errNum = ERROR_NONE;
		//채널에 입장시킴
		channel->InsertClientSession(m_ClientSession);

		//플레이어 데이터에 입장한 채널 세팅해줌
		m_ClientSession->GetPlayerData()->SetChannel(channel);
		m_ClientSession->GetPlayerData()->SetPlayerState(PLAYER_IN_CHANNEL);

		//채널 입장 성공 패킷 송신
		SendStream.write(&isSucces, sizeof(isSucces));
		SendStream.write(&errNum, sizeof(errNum));
		SendPacketWithSendStream(PK_ANS_WAITINGCHANNAL_CHANNAL_JOIN);

		LeaveCriticalSection(&ChannelInOutCS);
		return;
	}
	LeaveCriticalSection(&ChannelInOutCS);

	//채널 못찾음, 실패 패킷 송신
	bool isSucces = false;
	int errNum = ENTER_CHANNEL_CANT_FIND;
	SendStream.write(&isSucces, sizeof(isSucces));
	SendStream.write(&errNum, sizeof(errNum));
	SendPacketWithSendStream(PK_ANS_WAITINGCHANNAL_CHANNAL_JOIN);
}

void ClientSessionParser::ReqChannelSendMessage(T_PACKET * packet)
{
	SendStream.clear();
	//접속중인 채널의 클라이언트 세션에 메시지 보냄
	string message;
	SetRecvStream(packet);
	recvStream.read(message);

	string nickname = m_ClientSession->GetPlayerData()->GetPlayerNickname();
	
	int errNum = ERROR_NONE;
	SendStream.write(nickname);
	SendStream.write(message);

	T_PACKET pk(PK_RECV_CHANNAL_MESSAGE);
	pk.SetStream(SendStream);

	m_ClientSession->GetPlayerData()->
		GetChannel()->SendPacketToChannelMember(pk);
}

void ClientSessionParser::ReqChannelOut(T_PACKET * packet)
{
	SendStream.clear();
	Channel* channel = m_ClientSession->GetPlayerData()->GetChannel();
	
	//채널에서 클라세션을 지워줌으로서 채널 나감
	EnterCriticalSection(&ChannelInOutCS);
	if (channel->DeleteClientSession(m_ClientSession))
	{
		//채널 나가기 성공
		SLogPrintAtFile("%s : 채널 나가기 성공", m_ClientSession->GetPlayerData()->GetPlayerNickname().c_str());
		
		bool isSucces = true;
		int errNum = ERROR_NONE;
		
		//채널 나가기 성공 패킷 송신
		SendStream.write(&isSucces, sizeof(isSucces));
		SendStream.write(&errNum, sizeof(errNum));
		SendPacketWithSendStream(PK_ANS_CHANNAL_OUT);

		//채널에 아무도 없다면 채널 삭제함
		if (channel->ChannelErase())
		{
			SAFE_DELETE(channel);
		}
		LeaveCriticalSection(&ChannelInOutCS);
		return;
	}
	LeaveCriticalSection(&ChannelInOutCS);

	//채널 나가기 실패
	bool isSucces = false;
	int errNum = LEAVE_CHANNEL_CANT_FIND;
	SendStream.write(&isSucces, sizeof(isSucces));
	SendStream.write(&errNum, sizeof(errNum));
	SendPacketWithSendStream(PK_ANS_CHANNAL_OUT);
}

void ClientSessionParser::ReqExit(T_PACKET * packet)
{
	SendStream.clear();
	bool isSucces = true;
	int errNum = ERROR_NONE;
	
	SendStream.write(&isSucces, sizeof(isSucces));
	SendStream.write(&errNum, sizeof(errNum));
	SendPacketWithSendStream(PK_ANS_EXIT);
}

bool ClientSessionParser::PacketParsing(T_PACKET * const packet)
{
	switch (packet->type)
	{
	case PK_NONE:
		cout << "test : " << packet->buff << endl;
		break;

	case PK_REQ_LOGIN:
		ReqLogin(packet);
		break;

	case PK_REQ_CREATE_ID:
		ReqCreateID(packet);
		break;

	case PK_REQ_WAITINGCHANNAL_ENTER:
		ReqWatingChannelEnter(packet);
		break;

	case PK_REQ_WAITINGCHANNAL_CHREAT_CHANNAL:
		ReqWaitingChannelCreateChannel(packet);
		break;

	case PK_REQ_WAITINGCHANNAL_CHANNAL_JOIN:
		ReqWaitingChannelChannelJoin(packet);
		break;

	case PK_REQ_CHANNAL_SEND_MESSAGE:
		ReqChannelSendMessage(packet);
		break;

	case PK_REQ_CHANNAL_OUT:
		ReqChannelOut(packet);
		break;

	case PK_REQ_EXIT:
		ReqExit(packet);
		return false;

	default:
		ASSERT(false);
	}

	return true;
}
