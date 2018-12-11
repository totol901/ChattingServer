#include "stdafx.h"
#include "LoginChattingClientSessionParser.h"
#include <atlstr.h>

static CRITICAL_SECTION ChannelInOutCS;

LoginChattingClientSessionParser::LoginChattingClientSessionParser(ClientSession * clientSession)
	:ClientSessionParser(clientSession),
	m_LoginChattingClientSession((LoginChattingClientSession*)clientSession)
{
	InitializeCriticalSection(&ChannelInOutCS);
}

LoginChattingClientSessionParser::~LoginChattingClientSessionParser()
{
	DeleteCriticalSection(&ChannelInOutCS);
}

void LoginChattingClientSessionParser::ReqLogin(T_PACKET * packet)
{
	recvStream.clear();
	SendStream.clear();
	wstring id;
	wstring pw;
	bool isSuccess = false;
	int errorNum = 0;

	//stream에 받은 데이터 넣어줌
	const size_t headSize = sizeof(packet->Size) + sizeof(packet->type);
	recvStream.set(packet->buff, packet->Size - headSize);

	recvStream.wStringread(id);
	recvStream.wStringread(pw);

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

			SLogPrint(L"%s - 로그인 실패", id.c_str());

			//데이터베이스에 로그 남겨줌
			DATABASE->InsertUserLogQuery(id, L"로그인 실패");
		}
		else
		{
			//로그인 성공
			isSuccess = true;
			errorNum = ERROR_NONE;

			//stream에 보내줄 data 써줌
			wstring nickname = DATABASE->FindNickname(id);
			SendStream.write(&isSuccess, sizeof(isSuccess));
			SendStream.write(&errorNum, sizeof(errorNum));
			SendStream.write(nickname);

			SLogPrint(L"%s - 로그인 성공", id.c_str());

			//플레이어 데이터에 아이디 닉네임 저장
			m_LoginChattingClientSession->GetPlayerData()->SetPlayerID(id);
			m_LoginChattingClientSession->GetPlayerData()->SetPlayerNickname(nickname);
			m_LoginChattingClientSession->GetPlayerData()->SetPlayerState(PLAYER_LOGIN);

			//데이터베이스에 로그 남겨줌
			DATABASE->InsertUserLogQuery(id, L"로그인 완료");

			//로그인된 세션 풀에 넣음
			CLIENTSESSIONMANAGER->AddClientSessionID(id, m_LoginChattingClientSession);
		}
	}
	else
	{
		isSuccess = false;
		errorNum = LOGIN_ERROR_WRONG;

		//stream에 보내줄 data 써줌
		SendStream.write(&isSuccess, sizeof(isSuccess));
		SendStream.write(&errorNum, sizeof(errorNum));

		SLogPrint(L"%s - 로그인 실패", id.c_str());

		//데이터베이스에 로그 남겨줌
		DATABASE->InsertUserLogQuery(id, L"로그인 실패");
	}

	//패킷 송신
	m_SendPk.Clear();
	m_SendPk.type = PK_ANS_LOGIN;
	m_SendPk.SetStream(SendStream);
	m_ClientSession->SendPacket(m_SendPk);
}

void LoginChattingClientSessionParser::ReqCreateID(T_PACKET * packet)
{
	recvStream.clear();
	SendStream.clear();
	wstring id;
	wstring pw;
	wstring nickname;
	bool isSucces = false;
	int errNum = 0;

	const size_t headSize = sizeof(packet->Size) + sizeof(packet->type);
	recvStream.set(packet->buff, packet->Size - headSize);
	recvStream.wStringread(id);
	recvStream.wStringread(pw);
	recvStream.wStringread(nickname);

	if (DATABASE->InsertUserInfoQuery(id, pw, nickname))
	{
		//아이디 생성 성공
		isSucces = true;
		errNum = ERROR_NONE;

		SendStream.write(&isSucces, sizeof(isSucces));
		SendStream.write(&errNum, sizeof(errNum));

		//데이터베이스에 로그 남겨줌
		DATABASE->InsertUserLogQuery(id, L"아이디 생성");
	}
	else
	{
		isSucces = false;
		errNum = LOGIN_ERROR_CREATEID_ID_ALREADY_EXE;

		SendStream.write(&isSucces, sizeof(isSucces));
		SendStream.write(&errNum, sizeof(errNum));

		SLogPrint(L"%s - 아이디 생성 실패", id);
	}

	m_SendPk.Clear();
	m_SendPk.type = PK_ANS_CREATE_ID;
	m_SendPk.SetStream(SendStream);
	m_ClientSession->SendPacket(m_SendPk);
}

void LoginChattingClientSessionParser::ReqWatingChannelEnter(T_PACKET * packet)
{
	SLogPrint(L"%s : 채널 리스트 요청",
		m_LoginChattingClientSession->GetPlayerData()->GetPlayerID().c_str()
		);

	SendStream.clear();
	bool isSucces = true;
	int errNum = ERROR_NONE;
	wstring vecChannel = CHANNELMANAGER->GetChannelList();

	if (vecChannel.length() > PAKCET_BUFF_SIZE)
	{
		TCHAR tempStr[PAKCET_BUFF_SIZE];
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
	m_LoginChattingClientSession->GetPlayerData()->SetPlayerState(PLAYER_WAITING_CHANNEL);

	m_SendPk.Clear();
	m_SendPk.type = PK_ANS_WAITINGCHANNAL_ENTER;
	m_SendPk.SetStream(SendStream);
	m_LoginChattingClientSession->SendPacket(m_SendPk);
}

void LoginChattingClientSessionParser::ReqWaitingChannelCreateChannel(T_PACKET * packet)
{
	SendStream.clear();
	//채널 생성
	wstring channelName;
	SetRecvStream(packet);
	recvStream.wStringread(channelName);

	if (CHANNELMANAGER->MakeChannelWithChannelName(channelName))
	{
		SLogPrintAtFile(L"%s : 채널 생성 성공", m_LoginChattingClientSession->GetPlayerData()->GetPlayerNickname().c_str());

		//생성 완료됬다는 패킷 전송
		bool isSucces = true;
		int errNum = ERROR_NONE;

		SendStream.write(&isSucces, sizeof(isSucces));
		SendStream.write(&errNum, sizeof(errNum));
		SendPacketWithSendStream(PK_ANS_WAITINGCHANNAL_CHREAT_CHANNAL);

		//생성한 채널에 입장
		if (CHANNELMANAGER->ClientJoinTheChannel(m_LoginChattingClientSession, channelName))
		{
			SLogPrint(L"%s : %s, 채널 입장 성공", 
				m_LoginChattingClientSession->GetPlayerData()->GetPlayerID().c_str(),
				channelName.c_str());
		}
		else
		{
			SLogPrintAtFile(L"%s : !채널 생성후 입장 실패",
				m_LoginChattingClientSession->GetPlayerData()->GetPlayerID().c_str());
			ASSERT(FALSE);
		}

		return;
	}

	SLogPrint(L"%s : %s, 채널 생성 실패",
		m_LoginChattingClientSession->GetPlayerData()->GetPlayerID().c_str(),
		channelName.c_str());
	
	//생성 실패
	bool isSucces = false;
	int errNum = CREATE_CHANNEL_ALREADY_EXE;

	SendStream.write(&isSucces, sizeof(isSucces));
	SendStream.write(&errNum, sizeof(errNum));
	SendPacketWithSendStream(PK_ANS_WAITINGCHANNAL_CHREAT_CHANNAL);
}

void LoginChattingClientSessionParser::ReqWaitingChannelChannelJoin(T_PACKET * packet)
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
		m_LoginChattingClientSession->GetPlayerData()->SetChannel(channel);
		m_LoginChattingClientSession->GetPlayerData()->SetPlayerState(PLAYER_IN_CHANNEL);

		SLogPrint(L"%s : %s, 채널 입장 성공",
			m_LoginChattingClientSession->GetPlayerData()->GetPlayerID().c_str(),
			channel->GetName().c_str());

		//채널 입장 성공 패킷 송신
		SendStream.write(&isSucces, sizeof(isSucces));
		SendStream.write(&errNum, sizeof(errNum));
		SendPacketWithSendStream(PK_ANS_WAITINGCHANNAL_CHANNAL_JOIN);

		LeaveCriticalSection(&ChannelInOutCS);
		return;
	}
	LeaveCriticalSection(&ChannelInOutCS);

	SLogPrint(L"%s : !채널 입장 실패",
		m_LoginChattingClientSession->GetPlayerData()->GetPlayerID().c_str());

	//채널 못찾음, 실패 패킷 송신
	bool isSucces = false;
	int errNum = ENTER_CHANNEL_CANT_FIND;
	SendStream.write(&isSucces, sizeof(isSucces));
	SendStream.write(&errNum, sizeof(errNum));
	SendPacketWithSendStream(PK_ANS_WAITINGCHANNAL_CHANNAL_JOIN);
}

void LoginChattingClientSessionParser::ReqChannelSendMessage(T_PACKET * packet)
{
	SendStream.clear();
	//접속중인 채널의 클라이언트 세션에 메시지 보냄
	wstring message;
	SetRecvStream(packet);
	recvStream.wStringread(message);

	wstring nickname = m_LoginChattingClientSession->GetPlayerData()->GetPlayerNickname();
	
	int errNum = ERROR_NONE;
	SendStream.write(nickname);
	SendStream.write(message);

	T_PACKET pk(PK_RECV_CHANNAL_MESSAGE);
	pk.SetStream(SendStream);

	//채널에 속해있다면 채널의 맴버에게 송신
	Channel* tempChannel = m_LoginChattingClientSession->GetPlayerData()->GetChannel();
	if (tempChannel)
	{
		SLogPrint(L"%s : 채널에 메시지 송신",
			m_LoginChattingClientSession->GetPlayerData()->GetPlayerID().c_str());
		tempChannel->SendPacketToChannelMember(pk);
	}
}

void LoginChattingClientSessionParser::ReqChannelOut(T_PACKET * packet)
{
	SendStream.clear();
	Channel* channel = m_LoginChattingClientSession->GetPlayerData()->GetChannel();
	
	//채널에서 클라세션을 지워줌으로서 채널 나감
	EnterCriticalSection(&ChannelInOutCS);
	if (channel->DeleteClientSession(m_ClientSession))
	{
		//채널 나가기 성공
		SLogPrintAtFile(L"%s : 채널 나가기 성공", m_LoginChattingClientSession->GetPlayerData()->GetPlayerNickname().c_str());
		
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

	SLogPrint(L"%s : 채널나가기 실패",
		m_LoginChattingClientSession->GetPlayerData()->GetPlayerID().c_str());

	//채널 나가기 실패
	bool isSucces = false;
	int errNum = LEAVE_CHANNEL_CANT_FIND;
	SendStream.write(&isSucces, sizeof(isSucces));
	SendStream.write(&errNum, sizeof(errNum));
	SendPacketWithSendStream(PK_ANS_CHANNAL_OUT);
}

void LoginChattingClientSessionParser::ReqExit(T_PACKET * packet)
{
	SendStream.clear();
	bool isSucces = true;
	int errNum = ERROR_NONE;
	
	SendStream.write(&isSucces, sizeof(isSucces));
	SendStream.write(&errNum, sizeof(errNum));
	SendPacketWithSendStream(PK_ANS_EXIT);
}

void LoginChattingClientSessionParser::RecvMoveStart(NetworkSystem::T_PACKET * packet)
{
	recvStream.clear();
	float LocationX = 0.0f;
	float LocationY = 0.0f;
	float DirectionX = 0.0f;
	float DirectionY = 0.0f;
	float Velocity = 0.0f;

	//클라이언트의 현재 위치 방향 속도를 받아오고
	const size_t headSize = sizeof(packet->Size) + sizeof(packet->type);
	recvStream.set(packet->buff, packet->Size - headSize);
	recvStream.read(&LocationX, sizeof(float));
	recvStream.read(&LocationY, sizeof(float));
	recvStream.read(&DirectionX, sizeof(float));
	recvStream.read(&DirectionY, sizeof(float));
	recvStream.read(&Velocity, sizeof(float));

	//다른 클라이언트에 브로드케스팅 해주기 위해 ID 받아옴
	wstring ID = m_LoginChattingClientSession->GetPlayerData()->GetPlayerID();
	m_LoginChattingClientSession->GetPlayerData()->SetLocation(LocationX, LocationY);
	m_LoginChattingClientSession->GetPlayerData()->SetAcceleration(1.3f);

	SLogPrint(L"%s : MoveStart",
		m_LoginChattingClientSession->GetPlayerData()->GetPlayerID().c_str());

	//해당 클라이언트의 player가 아래 데이터를 가지고 이동한다고 브로드케스팅 함
	SendStream.clear();
	SendStream.write(ID);
	SendStream.write(&m_LoginChattingClientSession->GetPlayerData()->GetLocationX(),
		sizeof(float));
	SendStream.write(&m_LoginChattingClientSession->GetPlayerData()->GetLocationY(),
		sizeof(float));
	SendStream.write(&DirectionX, sizeof(float));
	SendStream.write(&DirectionY, sizeof(float));
	SendStream.write(&Velocity, sizeof(float));
	SendStream.write(&m_LoginChattingClientSession->GetPlayerData()->GetAcceleration(),
		sizeof(float));

	SendPacketWithSendStream(PK_RECV_MOVE_START);
}

void LoginChattingClientSessionParser::RecvMoveEnd(NetworkSystem::T_PACKET * packet)
{
	recvStream.clear();
	float LocationX = 0.0f;
	float LocationY = 0.0f;
	float DirectionX = 0.0f;
	float DirectionY = 0.0f;
	float Velocity = 0.0f;

	//클라이언트의 현재 위치 방향 속도를 받아오고
	const size_t headSize = sizeof(packet->Size) + sizeof(packet->type);
	recvStream.set(packet->buff, packet->Size - headSize);
	recvStream.read(&LocationX, sizeof(float));
	recvStream.read(&LocationY, sizeof(float));
	recvStream.read(&DirectionX, sizeof(float));
	recvStream.read(&DirectionY, sizeof(float));
	recvStream.read(&Velocity, sizeof(float));

	SLogPrint(L"%s : MoveEnd",
		m_LoginChattingClientSession->GetPlayerData()->GetPlayerID().c_str());

	//다른 클라이언트에 브로드케스팅 해주기 위해 ID 받아옴
	wstring ID = m_LoginChattingClientSession->GetPlayerData()->GetPlayerID();
	m_LoginChattingClientSession->GetPlayerData()->SetLocation(LocationX, LocationY);
	m_LoginChattingClientSession->GetPlayerData()->SetAcceleration(-1.3f);

	//해당 클라이언트의 player가 아래 데이터를 가지고 이동한다고 브로드케스팅 함
	SendStream.clear();
	SendStream.write(ID);
	SendStream.write(&m_LoginChattingClientSession->GetPlayerData()->GetLocationX(),
		sizeof(float));
	SendStream.write(&m_LoginChattingClientSession->GetPlayerData()->GetLocationY(),
		sizeof(float));
	SendStream.write(&DirectionX, sizeof(float));
	SendStream.write(&DirectionY, sizeof(float));
	SendStream.write(&Velocity, sizeof(float));
	SendStream.write(&m_LoginChattingClientSession->GetPlayerData()->GetAcceleration(),
		sizeof(float));

	SendPacketWithSendStream(PK_RECV_MOVE_END);
}

void LoginChattingClientSessionParser::RecvLocationRenewal(NetworkSystem::T_PACKET * packet)
{
	recvStream.clear();
	float LocationX = 0.0f;
	float LocationY = 0.0f;

	//클라이언트의 현재 위치 방향 속도를 받아오고
	const size_t headSize = sizeof(packet->Size) + sizeof(packet->type);
	recvStream.set(packet->buff, packet->Size - headSize);
	recvStream.read(&LocationX, sizeof(float));
	recvStream.read(&LocationY, sizeof(float));

	SLogPrint(L"%s : LocationRenewal",
		m_LoginChattingClientSession->GetPlayerData()->GetPlayerID().c_str());

	//다른 클라이언트에 브로드케스팅 해주기 위해 ID 받아옴
	wstring ID = m_LoginChattingClientSession->GetPlayerData()->GetPlayerID();
	m_LoginChattingClientSession->GetPlayerData()->SetLocation(LocationX, LocationY);
	
	//해당 클라이언트의 player가 아래 데이터를 가지고 이동한다고 브로드케스팅 함
	SendStream.clear();
	SendStream.write(ID);
	SendStream.write(&m_LoginChattingClientSession->GetPlayerData()->GetLocationX(),
		sizeof(float));
	SendStream.write(&m_LoginChattingClientSession->GetPlayerData()->GetLocationY(),
		sizeof(float));
	
	SendPacketWithSendStream(PK_RECV_LOCATION_RENEWAL);
}

bool LoginChattingClientSessionParser::PacketParsing(T_PACKET * const packet)
{
	EnterCriticalSection(&PacketParsingCS);

	switch (packet->type)
	{
	case PK_NONE:
	{
		TCHAR* str = (TCHAR*)packet->buff;

		wcout << str << endl;

		m_ClientSession->SendPacket(*packet);
	}
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
		LeaveCriticalSection(&PacketParsingCS);
		return false;

	case PK_SEND_MOVE_START:
		RecvMoveStart(packet);
		break;

	case PK_SEND_MOVE_END:
		RecvMoveEnd(packet);
		break;

	case PK_SEND_LOCATION_RENEWAL:
		RecvLocationRenewal(packet);
		break;

	default:
		ASSERT(false);
	}
	LeaveCriticalSection(&PacketParsingCS);

	return true;
}
