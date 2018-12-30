#include "stdafx.h"
#include "LoginChattingClientSessionParser.h"
#include <atlstr.h>

static CRITICAL_SECTION ChannelInOutCS;

static UINT COUNT = 0;

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

void LoginChattingClientSessionParser::ReqLogin(Packet * packetData)
{
	//recvStream.clear();
	SendStream.clear();
	wstring id;
	wstring pw;
	bool isSuccess = false;
	int errorNum = 0;

	PK_REQ_LOGIN* packet = (PK_REQ_LOGIN*)packetData;
	PK_ANS_LOGIN ansLoginPacket;

	//packet.Decode(*packetData);
	WCHAR strID[32] = { 0, };
	WCHAR strPW[32] = { 0, };
	Util::StrConvA2W((CHAR*)packet->ID.c_str(),
		strID, sizeof(strID));
	Util::StrConvA2W((CHAR*)packet->PW.c_str(),
		strPW, sizeof(strPW));
	id = strID;
	pw = strPW;
	
	//id, pw 쿼리 채크
	if (DATABASE->CheckUserInfoQuery(id, pw))
	{
		//로그인 중인지 확인
		if (CLIENTSESSIONMANAGER->FindClientSessionID(
			id) != nullptr)
		{
			//로그인 이미 함
			ansLoginPacket.IsSuccess = false;
			ansLoginPacket.ErrorNumber = LOGIN_ERROR_ALREADY_LOGIN;

			SLogPrint(L"%s - 로그인 실패, 이미 로그인 중", id.c_str());

			//데이터베이스에 로그 남겨줌
			DATABASE->InsertUserLogQuery(id, L"로그인 실패, 이미 로그인 중");

			//패킷 송신
			SendStream.clear();
			ansLoginPacket.Encode(SendStream);
			m_ClientSession->SynchronizationSendPacket(SendStream);

			//기존에 넣어둔 클라세션 빼서 없애줌, 소켓도 닫아버림
			closesocket(m_LoginChattingClientSession->GetSocket());
			CLIENTSESSIONMANAGER->DeleteClientSession(m_LoginChattingClientSession);
		}
		else
		{
			//로그인 성공
			ansLoginPacket.IsSuccess = true;
			ansLoginPacket.ErrorNumber = ERROR_NONE;

			//stream에 보내줄 data 써줌
			wstring nickname = DATABASE->FindNickname(id);
			CHAR strA[32] = { 0, };
			Util::StrConvW2A((WCHAR*)nickname.c_str(),
				strA, sizeof(strA));
			ansLoginPacket.Nickname = strA;

			SLogPrint(L"%s - 로그인 성공", id.c_str());

			//플레이어 데이터에 아이디 닉네임 저장
			m_LoginChattingClientSession->GetPlayerData()->SetPlayerID(id);
			m_LoginChattingClientSession->GetPlayerData()->SetPlayerNickname(nickname);
			m_LoginChattingClientSession->GetPlayerData()->SetPlayerState(PLAYER_LOGIN);

			//데이터베이스에 로그 남겨줌
			DATABASE->InsertUserLogQuery(id, L"로그인 완료");

			//로그인된 세션 풀에 넣음
			CLIENTSESSIONMANAGER->AddClientSessionID(id, m_LoginChattingClientSession);
			//기존에 넣어둔 클라세션 빼서 없애줌
			CLIENTSESSIONMANAGER->DeleteClientSession(m_LoginChattingClientSession);

			//패킷 송신
			SendStream.clear();
			ansLoginPacket.Encode(SendStream);

			m_ClientSession->SendPacket(SendStream);
		}
	}
	else
	{
		ansLoginPacket.IsSuccess = false;
		ansLoginPacket.ErrorNumber = LOGIN_ERROR_WRONG;

		SLogPrint(L"%s - 로그인 실패", id.c_str());

		//데이터베이스에 로그 남겨줌
		DATABASE->InsertUserLogQuery(id, L"로그인 실패");

		//패킷 송신
		SendStream.clear();
		ansLoginPacket.Encode(SendStream);
		m_ClientSession->SynchronizationSendPacket(SendStream);

		//기존에 넣어둔 클라세션 빼서 없애줌, 소켓도 닫아버림
		closesocket(m_LoginChattingClientSession->GetSocket());
		CLIENTSESSIONMANAGER->DeleteClientSession(m_LoginChattingClientSession);
	}
}

void LoginChattingClientSessionParser::ReqCreateID(Packet * packetData)
{
	//recvStream.clear();
	SendStream.clear();
	wstring id;
	wstring pw;
	wstring nickname;
	bool isSucces = false;
	int errNum = 0;

	PK_REQ_CREATE_ID* reqPacket = (PK_REQ_CREATE_ID*)packetData;
	PK_ANS_CREATE_ID ansPacket;

	//아스키->유니코드로 변환
	WCHAR strID[32] = { 0, };
	WCHAR strPW[32] = { 0, };
	WCHAR strNickname[32] = { 0, };
	Util::StrConvA2W((CHAR*)reqPacket->ID.c_str(),
		strID, sizeof(strID));
	Util::StrConvA2W((CHAR*)reqPacket->PW.c_str(),
		strPW, sizeof(strPW));
	Util::StrConvA2W((CHAR*)reqPacket->Nickname.c_str(),
		strNickname, sizeof(strNickname));
	id = strID;
	pw = strPW;
	nickname = strNickname;
	 
	if (DATABASE->InsertUserInfoQuery(id, pw, nickname))
	{
		//아이디 생성 성공
		ansPacket.IsSuccess = true;
		ansPacket.ErrorNumber = ERROR_NONE;

		//데이터베이스에 로그 남겨줌
		DATABASE->InsertUserLogQuery(id, L"아이디 생성");
	}
	else
	{
		ansPacket.IsSuccess = false;
		ansPacket.ErrorNumber = LOGIN_ERROR_CREATEID_ID_ALREADY_EXE;

		SLogPrint(L"%s - 아이디 생성 실패", id);
	}

	//ans패킷 송신
	ansPacket.Encode(SendStream);
	m_ClientSession->SendPacket(SendStream);
}

void LoginChattingClientSessionParser::ReqWatingChannelEnter(Packet * packetData)
{
	SLogPrint(L"%s : 채널 리스트 요청",
		m_LoginChattingClientSession->GetPlayerData()->GetPlayerID().c_str()
		);

	SendStream.clear();

	//ans 패킷 변수 업데이트
	PK_ANS_WAITINGCHANNAL_ENTER ansPacket;
	ansPacket.IsSuccess = true;
	ansPacket.ErrorNumber = ERROR_NONE;
	wstring vecChannel = CHANNELMANAGER->GetChannelList();
	CHAR strChannel[1024] = { 0, };
	Util::StrConvW2A((WCHAR*)vecChannel.c_str(),
		strChannel, sizeof(strChannel));
	ansPacket.ChannalList = strChannel;

	if (vecChannel.length() > PAKCET_BUFF_SIZE)
	{
		TCHAR tempStr[PAKCET_BUFF_SIZE];
		for (int i = 0; i < PAKCET_BUFF_SIZE; i++)
		{
			tempStr[i] = vecChannel[i];
		}
		vecChannel = tempStr;
	}

	//플레이어 데이터에 상태 업뎃
	m_LoginChattingClientSession->GetPlayerData()->SetPlayerState(PLAYER_WAITING_CHANNEL);

	ansPacket.Encode(SendStream);
	m_LoginChattingClientSession->SendPacket(SendStream);
}

void LoginChattingClientSessionParser::ReqWaitingChannelCreateChannel(Packet * packetData)
{
	SendStream.clear();

	PK_REQ_WAITINGCHANNAL_CHREAT_CHANNAL* reqPacket = (PK_REQ_WAITINGCHANNAL_CHREAT_CHANNAL*)packetData;
	PK_ANS_WAITINGCHANNAL_CHREAT_CHANNAL ansPacket;

	WCHAR str[1024] = { 0, };
	Util::StrConvA2W((CHAR*)reqPacket->ChannalName.c_str(),
		str, sizeof(str));
	//채널 생성
	wstring channelName = str;

	if (!m_LoginChattingClientSession->GetPlayerData()->GetChannel() &&
		CHANNELMANAGER->MakeChannelWithChannelName(channelName))
	{
		SLogPrintAtFile(L"%s : 채널 생성 성공", m_LoginChattingClientSession->GetPlayerData()->GetPlayerNickname().c_str());

		//생성 완료됬다는 패킷 전송
		ansPacket.IsSuccess = true;
		ansPacket.ErrorNumber = ERROR_NONE;
		ansPacket.Encode(SendStream);
		SendPacketWithSendStream(E_PK_ANS_WAITINGCHANNAL_CHREAT_CHANNAL);

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
	ansPacket.IsSuccess = false;
	ansPacket.ErrorNumber = CREATE_CHANNEL_ALREADY_EXE;
	ansPacket.Encode(SendStream);
	SendPacketWithSendStream(E_PK_ANS_WAITINGCHANNAL_CHREAT_CHANNAL);
}

void LoginChattingClientSessionParser::ReqWaitingChannelChannelJoin(Packet * packetData)
{
	SendStream.clear();

	PK_REQ_WAITINGCHANNAL_CHANNAL_JOIN* reqPacket = (PK_REQ_WAITINGCHANNAL_CHANNAL_JOIN*)packetData;
	PK_ANS_WAITINGCHANNAL_CHANNAL_JOIN ansPacket;

	//해당 채널 찾음
	UINT channelNum = reqPacket->ChannalNumber;

	//채널 찾음 유무에 따라 패킷 송신
	EnterCriticalSection(&ChannelInOutCS);
	Channel* channel = CHANNELMANAGER->FindChannelByID(channelNum);
	if (channel)
	{
		ansPacket.IsSuccess = true;
		ansPacket.ErrorNumber = ERROR_NONE;

		//플레이어 데이터에 입장한 채널 세팅해줌
		m_LoginChattingClientSession->GetPlayerData()->SetChannel(channel);
		m_LoginChattingClientSession->GetPlayerData()->SetPlayerState(PLAYER_IN_CHANNEL);

		//채널 들어온거 다른 클라에게 알림
		SendChannelJoinAnnounce();

		//현재 클라이언트에게 채널 내에 있는 다른 클라이언트 데이터 보내줌
		SendChannelData();

		//채널에 입장시킴
		channel->InsertClientSession(m_LoginChattingClientSession);

		SLogPrint(L"%s : %s, 채널 입장 성공",
			m_LoginChattingClientSession->GetPlayerData()->GetPlayerID().c_str(),
			channel->GetName().c_str());

		//채널 입장 성공 패킷 송신
		ansPacket.Encode(SendStream);
		SendPacketWithSendStream(E_PK_ANS_WAITINGCHANNAL_CHANNAL_JOIN);

		LeaveCriticalSection(&ChannelInOutCS);

		return;
	}
	LeaveCriticalSection(&ChannelInOutCS);

	SLogPrint(L"%s : !채널 입장 실패",
		m_LoginChattingClientSession->GetPlayerData()->GetPlayerID().c_str());

	//채널 못찾음, 실패 패킷 송신
	SendStream.clear();
	ansPacket.IsSuccess = false;
	ansPacket.ErrorNumber = ENTER_CHANNEL_CANT_FIND;
	ansPacket.Encode(SendStream);
	SendPacketWithSendStream(E_PK_ANS_WAITINGCHANNAL_CHANNAL_JOIN);
}

void LoginChattingClientSessionParser::ReqChannelSendMessage(Packet * packetData)
{
	SendStream.clear();

	PK_REQ_CHANNAL_SEND_MESSAGE* reqPacket = (PK_REQ_CHANNAL_SEND_MESSAGE*)packetData;
	PK_RECV_CHANNAL_MESSAGE recvPacket;

	//접속중인 채널의 클라이언트 세션에 메시지 보냄
	WCHAR str[1024] = { 0, };
	Util::StrConvA2W((CHAR*)reqPacket->Message.c_str(),
		str, sizeof(str));
	wstring message = str;

	wstring nickname = m_LoginChattingClientSession->GetPlayerData()->GetPlayerNickname();
	
	CHAR asiiNickName[32] = { 0, };
	Util::StrConvW2A((WCHAR*)nickname.c_str(),
		asiiNickName, sizeof(asiiNickName));
	recvPacket.Nickname = asiiNickName;
	recvPacket.Message = reqPacket->Message;

	//채널에 속해있다면 채널의 맴버에게 송신
	Channel* tempChannel = m_LoginChattingClientSession->GetPlayerData()->GetChannel();
	if (tempChannel)
	{
		SLogPrint(L"%s : 채널에 메시지 송신",
			m_LoginChattingClientSession->GetPlayerData()->GetPlayerID().c_str());
		recvPacket.Encode(SendStream);
		tempChannel->SendPacketToChannelMember(SendStream);
	}
}

void LoginChattingClientSessionParser::ReqChannelOut(Packet * packetData)
{
	SendStream.clear();

	PK_ANS_CHANNAL_OUT ansPacket;

	Channel* channel = m_LoginChattingClientSession->GetPlayerData()->GetChannel();
	
	//채널에서 클라세션을 지워줌으로서 채널 나감
	EnterCriticalSection(&ChannelInOutCS);
	if (channel->DeleteClientSession(m_LoginChattingClientSession))
	{
		//채널 나가기 성공
		SLogPrintAtFile(L"%s : 채널 나가기 성공", m_LoginChattingClientSession->GetPlayerData()->GetPlayerNickname().c_str());
		
		ansPacket.IsSuccess = true;
		ansPacket.ErrorNumber = ERROR_NONE;
		
		//채널 나가기 성공 패킷 송신
		ansPacket.Encode(SendStream);
		SendPacketWithSendStream(E_PK_ANS_CHANNAL_OUT);

		//채널 나간거 채널내의 다른 클라에게 알림
		SendChannelOutAnnounce();

		//플레이어의 채널 없는 것으로 바꿔줌
		m_LoginChattingClientSession->GetPlayerData()->SetChannel(nullptr);
		m_LoginChattingClientSession->GetPlayerData()->SetPlayerState(PLAYER_WAITING_CHANNEL);

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
	ansPacket.IsSuccess = false;
	ansPacket.ErrorNumber = LEAVE_CHANNEL_CANT_FIND;
	ansPacket.Encode(SendStream);
	SendPacketWithSendStream(E_PK_ANS_CHANNAL_OUT);
}

void LoginChattingClientSessionParser::ReqExit(Packet * packetData)
{
	PK_ANS_EXIT ansPacket;

	SendStream.clear();
	
	ansPacket.IsSuccess = true;
	ansPacket.ErrorNumber = ERROR_NONE;
	
	ansPacket.Encode(SendStream);
	SendPacketWithSendStream(E_PK_ANS_EXIT);
}

void LoginChattingClientSessionParser::RecvMoveStart(NetworkSystem::Packet * packetData)
{
	PK_SEND_MOVE_START* sendPacket = (PK_SEND_MOVE_START*)packetData;
	PK_RECV_MOVE_START recvPacket;

	//다른 클라이언트에 브로드케스팅 해주기 위해 ID 받아옴
	wstring ID = m_LoginChattingClientSession->GetPlayerData()->GetPlayerID();
	m_LoginChattingClientSession->GetPlayerData()->SetLocation(
		sendPacket->LocationX, sendPacket->LocationY);
	m_LoginChattingClientSession->GetPlayerData()->SetDirection(
		sendPacket->DirectionX, sendPacket->DirectionY);
	
	//if (fabsf(DirectionX + DirectionY - 1.0f) <= FLT_EPSILON)
	//{
	//	m_LoginChattingClientSession->GetPlayerData()->SetDirection(DirectionX, DirectionY);
	//}
	//else
	//{
	//	m_LoginChattingClientSession->GetPlayerData()->SetDirection(DirectionX, DirectionY);
	//}

	//속도 지정
	m_LoginChattingClientSession->GetPlayerData()->SetVelocity(10.0f);
	
	//해당 클라이언트의 player가 아래 데이터를 가지고 이동한다고 브로드케스팅 함
	SendStream.clear();
	CHAR asiiID[32] = { 0, };
	Util::StrConvW2A((WCHAR*)ID.c_str(),
		asiiID, sizeof(asiiID));
	recvPacket.ID = asiiID;
	recvPacket.LocationX = m_LoginChattingClientSession->GetPlayerData()->GetLocationX();
	recvPacket.LocationY = m_LoginChattingClientSession->GetPlayerData()->GetLocationY();
	recvPacket.DirectionX = m_LoginChattingClientSession->GetPlayerData()->GetDirectionX();
	recvPacket.DirectionY = m_LoginChattingClientSession->GetPlayerData()->GetDirectionY();
	recvPacket.Velocity = m_LoginChattingClientSession->GetPlayerData()->GetVelocity();
	//SendStream.write(&COUNT, sizeof(COUNT));

	//SLogPrint(L"%s : MoveStart, LocationX : %f, LocationY : %f, Time : %d",
	//	m_LoginChattingClientSession->GetPlayerData()->GetPlayerID().c_str(),
	//	m_LoginChattingClientSession->GetPlayerData()->GetLocationX(),
	//	m_LoginChattingClientSession->GetPlayerData()->GetLocationY(),
	//	++COUNT
	//);

	recvPacket.Encode(SendStream);
	//채널내에있는 클라이언트에 패킷 보냄
	Channel* tempChannel = m_LoginChattingClientSession->GetPlayerData()->GetChannel();
	if (tempChannel)
	{
		tempChannel->SendPacketToChannelMember(SendStream);
	}

	//시작 시간 넣어둠
	m_LoginChattingClientSession->GetPlayerData()->m_DoStartTimePoint = TIMER->GetNowHighTimePoint();
}

void LoginChattingClientSessionParser::RecvMoveEnd(NetworkSystem::Packet * packetData)
{
	PK_SEND_MOVE_END* sendPacket = (PK_SEND_MOVE_END*)packetData;
	PK_RECV_MOVE_END recvPacket;

	//다른 클라이언트에 브로드케스팅 해주기 위해 ID 받아옴
	wstring ID = m_LoginChattingClientSession->GetPlayerData()->GetPlayerID();
	m_LoginChattingClientSession->GetPlayerData()->SetLocation(
		sendPacket->LocationX, sendPacket->LocationY);
	if (fabsf(sendPacket->DirectionX + sendPacket->DirectionY - 1.0f) <= FLT_EPSILON)
	{
		m_LoginChattingClientSession->GetPlayerData()->SetDirection(
			sendPacket->DirectionX, sendPacket->DirectionY);
	}
	else
	{
		m_LoginChattingClientSession->GetPlayerData()->SetDirection(
			sendPacket->DirectionX, sendPacket->DirectionY);
	}
	m_LoginChattingClientSession->GetPlayerData()->SetVelocity(0.0f);

	//해당 클라이언트의 player가 아래 데이터를 가지고 이동한다고 브로드케스팅 함
	SendStream.clear();
	CHAR asiiID[32] = { 0, };
	Util::StrConvW2A((WCHAR*)ID.c_str(),
		asiiID, sizeof(asiiID));
	recvPacket.ID = asiiID;
	recvPacket.LocationX = m_LoginChattingClientSession->GetPlayerData()->GetLocationX();
	recvPacket.LocationY = m_LoginChattingClientSession->GetPlayerData()->GetLocationY();
	recvPacket.DirectionX = m_LoginChattingClientSession->GetPlayerData()->GetDirectionX();
	recvPacket.DirectionY = m_LoginChattingClientSession->GetPlayerData()->GetDirectionY();
	
	//COUNT++;
	//SendStream.write(&COUNT, sizeof(COUNT));

	SLogPrint(L"%s : MoveEnd, LocationX : %f, LocationY : %f, Time : %d",
		m_LoginChattingClientSession->GetPlayerData()->GetPlayerID().c_str(),
		m_LoginChattingClientSession->GetPlayerData()->GetLocationX(),
		m_LoginChattingClientSession->GetPlayerData()->GetLocationY(),
		++COUNT
	);

	recvPacket.Encode(SendStream);
	Channel* tempChannel = m_LoginChattingClientSession->GetPlayerData()->GetChannel();
	if (tempChannel)
	{
		tempChannel->SendPacketToChannelMember(SendStream);
	}
	//시작 시간 넣어둠
	m_LoginChattingClientSession->GetPlayerData()->m_DoStartTimePoint = TIMER->GetNowHighTimePoint();
}

void LoginChattingClientSessionParser::RecvLocationRenewal(NetworkSystem::Packet * packetData)
{
	PK_SEND_LOCATION_RENEWAL* sendPacket = (PK_SEND_LOCATION_RENEWAL*)packetData;
	PK_RECV_LOCATION_RENEWAL recvPacket;

	sendPacket->DirectionX *= 0.5f;
	sendPacket->DirectionY *= 0.5f;

	SLogPrint(L"%s : LocationRenewal",
		m_LoginChattingClientSession->GetPlayerData()->GetPlayerID().c_str());

	//다른 클라이언트에 브로드케스팅 해주기 위해 ID 받아옴
	wstring ID = m_LoginChattingClientSession->GetPlayerData()->GetPlayerID();
	m_LoginChattingClientSession->GetPlayerData()->SetLocation(
		sendPacket->LocationX, sendPacket->LocationY);
	if (fabsf(sendPacket->DirectionX + sendPacket->DirectionY - 1.0f) <= FLT_EPSILON)
	{
		m_LoginChattingClientSession->GetPlayerData()->SetDirection(
			sendPacket->DirectionX, sendPacket->DirectionY);
	}
	else
	{
		m_LoginChattingClientSession->GetPlayerData()->SetDirection(
			sendPacket->DirectionX, sendPacket->DirectionY);
	}
	
	//해당 클라이언트의 player가 아래 데이터를 가지고 이동한다고 브로드케스팅 함
	SendStream.clear();
	CHAR asiiID[32] = { 0, };
	Util::StrConvW2A((WCHAR*)ID.c_str(),
		asiiID, sizeof(asiiID));
	recvPacket.ID = asiiID;
	recvPacket.LocationX = m_LoginChattingClientSession->GetPlayerData()->GetLocationX();
	recvPacket.LocationY = m_LoginChattingClientSession->GetPlayerData()->GetLocationY();
	recvPacket.DirectionX = m_LoginChattingClientSession->GetPlayerData()->GetDirectionX();
	recvPacket.DirectionY = m_LoginChattingClientSession->GetPlayerData()->GetDirectionY();
	
	recvPacket.Encode(SendStream);
	Channel* tempChannel = m_LoginChattingClientSession->GetPlayerData()->GetChannel();
	if (tempChannel)
	{
		tempChannel->SendPacketToChannelMember(SendStream);
	}
}

void LoginChattingClientSessionParser::RecvServerTime(NetworkSystem::Packet * packetData)
{
	//m_LoginChattingClientSession->RecvHeartBeat();

	//SLogPrint(L"%s의 핑 : %d",
	//	m_LoginChattingClientSession->GetPlayerData()->GetPlayerID().c_str(),
	//	m_LoginChattingClientSession->GetPing());
 
	PK_SEND_SERVERTIME sendPacket;
	//서버시간 송신
	//m_SendPk.Clear();
	SendStream.clear();
	
	//m_SendPk.type = PK_RECV_SERVERTIME;
	sendPacket.ServerTime = (int)TIMER->GetNowTime_t();
	sendPacket.Encode(SendStream);
	m_ClientSession->SendPacket(SendStream);

	m_LoginChattingClientSession->SendHeartBeat();
}

void LoginChattingClientSessionParser::SendChannelJoinAnnounce()
{
	PK_RECV_CHANNAL_JOIN_ANNOUNCE packet;
	//다른 클라이언트에 브로드케스팅 해주기 위해 ID 받아옴
	wstring ID = m_LoginChattingClientSession->GetPlayerData()->GetPlayerID();
	CHAR asiiID[32] = { 0, };
	Util::StrConvW2A((WCHAR*)ID.c_str(),
		asiiID, sizeof(asiiID));

	//해당 클라이언트의 player가 아래 데이터를 가지고 이동한다고 브로드케스팅 함
	packet.ID = asiiID;

	SendStream.clear();
	packet.Encode(SendStream);
	Channel* tempChannel = m_LoginChattingClientSession->GetPlayerData()->GetChannel();
	if (tempChannel)
	{
		SLogPrint(L"%s : SendChannelJoinAnnounce",
			m_LoginChattingClientSession->GetPlayerData()->GetPlayerID().c_str());

		tempChannel->SendPacketToChannelMember(SendStream);
	}
}

void LoginChattingClientSessionParser::SendChannelOutAnnounce()
{
	PK_RECV_CHANNAL_OUT_ANNOUNCE recvPacket;

	//다른 클라이언트에 브로드케스팅 해주기 위해 ID 받아옴
	wstring ID = m_LoginChattingClientSession->GetPlayerData()->GetPlayerID();
	CHAR asiiID[32] = { 0, };
	Util::StrConvW2A((WCHAR*)ID.c_str(),
		asiiID, sizeof(asiiID));
	recvPacket.ID = asiiID;

	//해당 클라이언트의 player가 아래 데이터를 가지고 이동한다고 브로드케스팅 함
	recvPacket.Encode(SendStream);
	Channel* tempChannel = m_LoginChattingClientSession->GetPlayerData()->GetChannel();
	if (tempChannel)
	{
		SLogPrint(L"%s : SendChannelOutAnnounce",
			m_LoginChattingClientSession->GetPlayerData()->GetPlayerID().c_str());
		tempChannel->SendPacketToChannelMember(SendStream);
	}
}

void LoginChattingClientSessionParser::SendChannelData()
{
	Channel* tempChannel = m_LoginChattingClientSession->GetPlayerData()->GetChannel();
	tempChannel->SendPacketChannelData(m_LoginChattingClientSession);
}

bool LoginChattingClientSessionParser::PacketParsing(Packet * packetData)
{
	if(ClientSessionParser::PacketParsing(packetData))
	{
		return true;
	}

	EnterCriticalSection(&PacketParsingCS);

	switch (packetData->Type())
	{
	case E_PK_NONE:
	{
		PK_NONE* packet = (PK_NONE*)packetData;
		CHAR* str = (CHAR*)packet->test.c_str();
		TCHAR tStr[1024] = { 0, };
		Util::StrConvA2T(str, tStr, sizeof(tStr));
		wcout << str << endl;

		SendStream.clear();
		packet->Encode(SendStream);
		m_ClientSession->SendPacket(SendStream);
	}
		break;

	case E_PK_REQ_LOGIN:
		ReqLogin(packetData);
		break;

	case E_PK_REQ_CREATE_ID:
		ReqCreateID(packetData);
		break;

	case E_PK_REQ_WAITINGCHANNAL_ENTER:
		ReqWatingChannelEnter(packetData);
		break;

	case E_PK_REQ_WAITINGCHANNAL_CHREAT_CHANNAL:
		ReqWaitingChannelCreateChannel(packetData);
		break;

	case E_PK_REQ_WAITINGCHANNAL_CHANNAL_JOIN:
		ReqWaitingChannelChannelJoin(packetData);
		break;

	case E_PK_REQ_CHANNAL_SEND_MESSAGE:
		ReqChannelSendMessage(packetData);
		break;

	case E_PK_REQ_CHANNAL_OUT:
		ReqChannelOut(packetData);
		break;

	case E_PK_REQ_EXIT:
		ReqExit(packetData);
		LeaveCriticalSection(&PacketParsingCS);
		return false;

	case E_PK_SEND_MOVE_START:
		RecvMoveStart(packetData);
		break;

	case E_PK_SEND_MOVE_END:
		RecvMoveEnd(packetData);
		break;

	case E_PK_SEND_LOCATION_RENEWAL:
		RecvLocationRenewal(packetData);
		break;

	case E_PK_SEND_SERVERTIME:
		RecvServerTime(packetData);
		break;

	default:
		ASSERT(false);
	}

	LeaveCriticalSection(&PacketParsingCS);

	return true;
}
