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

void ClientSessionParser::ReqLogin(T_PACKET * packet)
{
	recvStream.clear();
	SendStream.clear();
	char id[15] = { 0, };
	char pw[15] = { 0, };
	char nickname[15] = { 0, };
	bool isSuccess = false;
	int errorNum = 0;
	T_PACKET pk(PK_ANS_LOGIN);

	//stream에 받은 데이터 넣어줌
	recvStream.set(packet->buff, PAKCET_BUFF_SIZE);
	recvStream.read(id, sizeof(id));
	recvStream.read(pw, sizeof(pw));

	//id, pw 쿼리 채크
	if (DATABASE->CheckUserInfoQuery(id, pw))
	{
		//로그인 성공
		isSuccess = true;
		errorNum = 0;

		//stream에 보내줄 data 써줌
		string nickname = DATABASE->FindNickname(id);
		const char* str = nickname.c_str();
		SendStream.write(&isSuccess, sizeof(isSuccess));
		SendStream.write(&errorNum, sizeof(errorNum));
		SendStream.write((void*)str, nickname.size());

		SLogPrint("%s - 로그인 성공", id);

		//플레이어 데이터에 아이디 닉네임 저장
		m_ClientSession->GetPlayerData()->SetPlayerID(id);
		m_ClientSession->GetPlayerData()->SetPlayerNickname(str);
		m_ClientSession->GetPlayerData()->SetPlayerState(PLAYER_LOGIN);

		//데이터베이스에 로그 남겨줌
		DATABASE->InsertUserLogQuery(id, "로그인 완료");
	}
	else
	{
		isSuccess = false;
		errorNum = 0;

		//stream에 보내줄 data 써줌
		SendStream.write(&isSuccess, sizeof(isSuccess));
		SendStream.write(&errorNum, sizeof(errorNum));

		SLogPrint("%s - 로그인 실패", id);

		//데이터베이스에 로그 남겨줌
		DATABASE->InsertUserLogQuery(id, "로그인 시도");
	}

	//패킷 송신
	pk.SetStream(SendStream);
	m_ClientSession->SendPacket(pk);
}

void ClientSessionParser::ReqCreateID(T_PACKET * packet)
{
	char id[15] = { 0, };
	char pw[15] = { 0, };
	char nickname[15] = { 0, };
	bool isSucces = false;
	int errNum = 0;

	recvStream.set(packet->buff, PAKCET_BUFF_SIZE);
	recvStream.read(id, sizeof(id));
	recvStream.read(pw, sizeof(pw));
	recvStream.read(nickname, sizeof(nickname));
	T_PACKET pk(PK_ANS_CREATE_ID);

	if (DATABASE->InsertUserInfoQuery(id, pw, nickname))
	{
		//아이디 생성 성공
		isSucces = true;
		errNum = 0;

		SendStream.write(&isSucces, sizeof(isSucces));
		SendStream.write(&errNum, sizeof(errNum));

		//데이터베이스에 로그 남겨줌
		DATABASE->InsertUserLogQuery(id, "아이디 생성");
	}
	else
	{
		isSucces = false;
		errNum = 0;

		SendStream.write(&isSucces, sizeof(isSucces));
		SendStream.write(&errNum, sizeof(errNum));

		SLogPrint("%s - 아이디 생성 실패", id);
	}

	m_ClientSession->SendPacket(pk);
}

void ClientSessionParser::ReqWatingChannelEnter(T_PACKET * packet)
{
	bool isSucces = true;
	int errNum = 0;
	string vecChannel = CHANNELMANAGER->GetChannelList();

	SendStream.write(&isSucces, sizeof(isSucces));
	SendStream.write(&errNum, sizeof(errNum));
	SendStream.write((void*)vecChannel.c_str(), 64);

	//플레이어 데이터에 상태 업뎃
	m_ClientSession->GetPlayerData()->SetPlayerState(PLAYER_WAITING_CHANNEL);

	T_PACKET pk(PK_ANS_WAITINGCHANNAL_ENTER);
	pk.SetStream(SendStream);

	m_ClientSession->SendPacket(pk);
}

void ClientSessionParser::ReqWaitingChannelCreateChannel(T_PACKET * packet)
{
	//채널 생성
	char channelName[15] = { 0, };
	recvStream.set(packet->buff, PAKCET_BUFF_SIZE);
	recvStream.read(channelName, sizeof(channelName));

	if (CHANNELMANAGER->MakeChannel(channelName))
	{
		//생성 완료됬다는 패킷 전송
		bool isSucces = true;
		int errNum = 0;

		SendStream.write(&isSucces, sizeof(isSucces));
		SendStream.write(&errNum, sizeof(errNum));

		T_PACKET pk(PK_ANS_WAITINGCHANNAL_CHREAT_CHANNAL);
		pk.SetStream(SendStream);

		m_ClientSession->SendPacket(pk);

		//생성한 채널에 입장
		Channel* channel = CHANNELMANAGER->FindChannelByName(channelName);
		if (channel)
		{
			bool isSucces = true;
			int errNum = 0;
			//채널에 입장시킴
			channel->InsertClientSession(m_ClientSession);

			//플레이어 데이터에 입장한 채널 세팅해줌
			m_ClientSession->GetPlayerData()->SetChannel(channel);
			m_ClientSession->GetPlayerData()->SetPlayerState(PLAYER_IN_CHANNEL);

			//채널 입장 성공 패킷 송신
			SendStream.write(&isSucces, sizeof(isSucces));
			SendStream.write(&errNum, sizeof(errNum));
			pk.SetStream(SendStream);
			m_ClientSession->SendPacket(pk);
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
	int errNum = 0;

	SendStream.write(&isSucces, sizeof(isSucces));
	SendStream.write(&errNum, sizeof(errNum));

	T_PACKET pk(PK_ANS_WAITINGCHANNAL_CHREAT_CHANNAL);
	pk.SetStream(SendStream);

	m_ClientSession->SendPacket(pk);
}

void ClientSessionParser::ReqWaitingChannelChannelJoin(T_PACKET * packet)
{
	//해당 채널 찾음
	UINT channelNum = 0;
	recvStream.set(packet->buff, PAKCET_BUFF_SIZE);
	recvStream.read(&channelNum, sizeof(channelNum));

	//채널 찾음 유무에 따라 패킷 송싱
	T_PACKET pk(PK_ANS_WAITINGCHANNAL_CHANNAL_JOIN);

	EnterCriticalSection(&ChannelInOutCS);
	Channel* channel = CHANNELMANAGER->FindChannelByID(channelNum);
	if (channel)
	{
		bool isSucces = true;
		int errNum = 0;
		//채널에 입장시킴
		channel->InsertClientSession(m_ClientSession);

		//플레이어 데이터에 입장한 채널 세팅해줌
		m_ClientSession->GetPlayerData()->SetChannel(channel);
		m_ClientSession->GetPlayerData()->SetPlayerState(PLAYER_IN_CHANNEL);

		//채널 입장 성공 패킷 송신
		SendStream.write(&isSucces, sizeof(isSucces));
		SendStream.write(&errNum, sizeof(errNum));
		pk.SetStream(SendStream);
		m_ClientSession->SendPacket(pk);
		LeaveCriticalSection(&ChannelInOutCS);
		return;
	}
	LeaveCriticalSection(&ChannelInOutCS);

	//채널 못찾음, 실패 패킷 송신
	bool isSucces = false;
	int errNum = 0;
	SendStream.write(&isSucces, sizeof(isSucces));
	SendStream.write(&errNum, sizeof(errNum));
	pk.SetStream(SendStream);
	m_ClientSession->SendPacket(pk);
}

void ClientSessionParser::ReqChannelSendMessage(T_PACKET * packet)
{
	//접속중인 채널의 클라이언트 세션에 메시지 보냄
	char message[64] = {0, };
	recvStream.set(packet->buff, PAKCET_BUFF_SIZE);
	recvStream.read(message, sizeof(message));

	T_PACKET pk(PK_RECV_CHANNAL_MESSAGE);
	char nickname[15] = {0,};
	memcpy(nickname, m_ClientSession->GetPlayerData()->GetPlayerNickname().c_str(),
		m_ClientSession->GetPlayerData()->GetPlayerNickname().size());
	int errNum = 0;
	SendStream.write(nickname, sizeof(nickname));
	SendStream.write(message, sizeof(message));
	pk.SetStream(SendStream);

	m_ClientSession->GetPlayerData()->
		GetChannel()->SendPacketToChannelMember(pk);
}

void ClientSessionParser::ReqChannelOut(T_PACKET * packet)
{
	Channel* channel = m_ClientSession->GetPlayerData()->GetChannel();
	T_PACKET pk(PK_ANS_CHANNAL_OUT);
	
	//채널에서 클라세션을 지워줌으로서 채널 나감
	EnterCriticalSection(&ChannelInOutCS);
	if (channel->DeleteClientSession(m_ClientSession))
	{
		//채널 나가기 성공
		SLogPrintAtFile("%d : 채널 나가기 성공", m_ClientSession->GetPlayerData()->GetPlayerNickname().c_str());
		
		bool isSucces = true;
		int errNum = 0;
		
		//채널 나가기 성공 패킷 송신
		SendStream.write(&isSucces, sizeof(isSucces));
		SendStream.write(&errNum, sizeof(errNum));
		pk.SetStream(SendStream);
		m_ClientSession->SendPacket(pk);

		//채널에 아무도 없다면 채널 삭제함
		if (channel->IsChannelEmpty())
		{
			CHANNELMANAGER->DeleteChannelByID(channel->GetID());
			SAFE_DELETE(channel);
		}
		LeaveCriticalSection(&ChannelInOutCS);
		return;
	}
	LeaveCriticalSection(&ChannelInOutCS);

	//채널 나가기 실패
	bool isSucces = false;
	int errNum = 0;
	SendStream.write(&isSucces, sizeof(isSucces));
	SendStream.write(&errNum, sizeof(errNum));
	pk.SetStream(SendStream);
	m_ClientSession->SendPacket(pk);
}

void ClientSessionParser::ReqExit(T_PACKET * packet)
{
	bool isSucces = true;
	int errNum = 0;
	
	T_PACKET pk(PK_ANS_EXIT);

	SendStream.write(&isSucces, sizeof(isSucces));
	SendStream.write(&errNum, sizeof(errNum));
	pk.SetStream(SendStream);

	m_ClientSession->SendPacket(pk);
}

bool ClientSessionParser::PacketParsing(T_PACKET * const packet)
{
	recvStream.clear();
	SendStream.clear();

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
		m_ClientSession->SetConnected(false);
		return false;
	}

	return true;
}
