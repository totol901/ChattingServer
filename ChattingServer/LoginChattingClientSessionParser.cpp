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
	
	//id, pw ���� äũ
	if (DATABASE->CheckUserInfoQuery(id, pw))
	{
		//�α��� ������ Ȯ��
		if (CLIENTSESSIONMANAGER->FindClientSessionID(
			id) != nullptr)
		{
			//�α��� �̹� ��
			ansLoginPacket.IsSuccess = false;
			ansLoginPacket.ErrorNumber = LOGIN_ERROR_ALREADY_LOGIN;

			SLogPrint(L"%s - �α��� ����, �̹� �α��� ��", id.c_str());

			//�����ͺ��̽��� �α� ������
			DATABASE->InsertUserLogQuery(id, L"�α��� ����, �̹� �α��� ��");

			//��Ŷ �۽�
			SendStream.clear();
			ansLoginPacket.Encode(SendStream);
			m_ClientSession->SynchronizationSendPacket(SendStream);

			//������ �־�� Ŭ�󼼼� ���� ������, ���ϵ� �ݾƹ���
			closesocket(m_LoginChattingClientSession->GetSocket());
			CLIENTSESSIONMANAGER->DeleteClientSession(m_LoginChattingClientSession);
		}
		else
		{
			//�α��� ����
			ansLoginPacket.IsSuccess = true;
			ansLoginPacket.ErrorNumber = ERROR_NONE;

			//stream�� ������ data ����
			wstring nickname = DATABASE->FindNickname(id);
			CHAR strA[32] = { 0, };
			Util::StrConvW2A((WCHAR*)nickname.c_str(),
				strA, sizeof(strA));
			ansLoginPacket.Nickname = strA;

			SLogPrint(L"%s - �α��� ����", id.c_str());

			//�÷��̾� �����Ϳ� ���̵� �г��� ����
			m_LoginChattingClientSession->GetPlayerData()->SetPlayerID(id);
			m_LoginChattingClientSession->GetPlayerData()->SetPlayerNickname(nickname);
			m_LoginChattingClientSession->GetPlayerData()->SetPlayerState(PLAYER_LOGIN);

			//�����ͺ��̽��� �α� ������
			DATABASE->InsertUserLogQuery(id, L"�α��� �Ϸ�");

			//�α��ε� ���� Ǯ�� ����
			CLIENTSESSIONMANAGER->AddClientSessionID(id, m_LoginChattingClientSession);
			//������ �־�� Ŭ�󼼼� ���� ������
			CLIENTSESSIONMANAGER->DeleteClientSession(m_LoginChattingClientSession);

			//��Ŷ �۽�
			SendStream.clear();
			ansLoginPacket.Encode(SendStream);

			m_ClientSession->SendPacket(SendStream);
		}
	}
	else
	{
		ansLoginPacket.IsSuccess = false;
		ansLoginPacket.ErrorNumber = LOGIN_ERROR_WRONG;

		SLogPrint(L"%s - �α��� ����", id.c_str());

		//�����ͺ��̽��� �α� ������
		DATABASE->InsertUserLogQuery(id, L"�α��� ����");

		//��Ŷ �۽�
		SendStream.clear();
		ansLoginPacket.Encode(SendStream);
		m_ClientSession->SynchronizationSendPacket(SendStream);

		//������ �־�� Ŭ�󼼼� ���� ������, ���ϵ� �ݾƹ���
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

	//�ƽ�Ű->�����ڵ�� ��ȯ
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
		//���̵� ���� ����
		ansPacket.IsSuccess = true;
		ansPacket.ErrorNumber = ERROR_NONE;

		//�����ͺ��̽��� �α� ������
		DATABASE->InsertUserLogQuery(id, L"���̵� ����");
	}
	else
	{
		ansPacket.IsSuccess = false;
		ansPacket.ErrorNumber = LOGIN_ERROR_CREATEID_ID_ALREADY_EXE;

		SLogPrint(L"%s - ���̵� ���� ����", id);
	}

	//ans��Ŷ �۽�
	ansPacket.Encode(SendStream);
	m_ClientSession->SendPacket(SendStream);
}

void LoginChattingClientSessionParser::ReqWatingChannelEnter(Packet * packetData)
{
	SLogPrint(L"%s : ä�� ����Ʈ ��û",
		m_LoginChattingClientSession->GetPlayerData()->GetPlayerID().c_str()
		);

	SendStream.clear();

	//ans ��Ŷ ���� ������Ʈ
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

	//�÷��̾� �����Ϳ� ���� ����
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
	//ä�� ����
	wstring channelName = str;

	if (!m_LoginChattingClientSession->GetPlayerData()->GetChannel() &&
		CHANNELMANAGER->MakeChannelWithChannelName(channelName))
	{
		SLogPrintAtFile(L"%s : ä�� ���� ����", m_LoginChattingClientSession->GetPlayerData()->GetPlayerNickname().c_str());

		//���� �Ϸ��ٴ� ��Ŷ ����
		ansPacket.IsSuccess = true;
		ansPacket.ErrorNumber = ERROR_NONE;
		ansPacket.Encode(SendStream);
		SendPacketWithSendStream(E_PK_ANS_WAITINGCHANNAL_CHREAT_CHANNAL);

		//������ ä�ο� ����
		if (CHANNELMANAGER->ClientJoinTheChannel(m_LoginChattingClientSession, channelName))
		{
			SLogPrint(L"%s : %s, ä�� ���� ����", 
				m_LoginChattingClientSession->GetPlayerData()->GetPlayerID().c_str(),
				channelName.c_str());
		}
		else
		{
			SLogPrintAtFile(L"%s : !ä�� ������ ���� ����",
				m_LoginChattingClientSession->GetPlayerData()->GetPlayerID().c_str());
			ASSERT(FALSE);
		}

		return;
	}

	SLogPrint(L"%s : %s, ä�� ���� ����",
		m_LoginChattingClientSession->GetPlayerData()->GetPlayerID().c_str(),
		channelName.c_str());
	
	//���� ����
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

	//�ش� ä�� ã��
	UINT channelNum = reqPacket->ChannalNumber;

	//ä�� ã�� ������ ���� ��Ŷ �۽�
	EnterCriticalSection(&ChannelInOutCS);
	Channel* channel = CHANNELMANAGER->FindChannelByID(channelNum);
	if (channel)
	{
		ansPacket.IsSuccess = true;
		ansPacket.ErrorNumber = ERROR_NONE;

		//�÷��̾� �����Ϳ� ������ ä�� ��������
		m_LoginChattingClientSession->GetPlayerData()->SetChannel(channel);
		m_LoginChattingClientSession->GetPlayerData()->SetPlayerState(PLAYER_IN_CHANNEL);

		//ä�� ���°� �ٸ� Ŭ�󿡰� �˸�
		SendChannelJoinAnnounce();

		//���� Ŭ���̾�Ʈ���� ä�� ���� �ִ� �ٸ� Ŭ���̾�Ʈ ������ ������
		SendChannelData();

		//ä�ο� �����Ŵ
		channel->InsertClientSession(m_LoginChattingClientSession);

		SLogPrint(L"%s : %s, ä�� ���� ����",
			m_LoginChattingClientSession->GetPlayerData()->GetPlayerID().c_str(),
			channel->GetName().c_str());

		//ä�� ���� ���� ��Ŷ �۽�
		ansPacket.Encode(SendStream);
		SendPacketWithSendStream(E_PK_ANS_WAITINGCHANNAL_CHANNAL_JOIN);

		LeaveCriticalSection(&ChannelInOutCS);

		return;
	}
	LeaveCriticalSection(&ChannelInOutCS);

	SLogPrint(L"%s : !ä�� ���� ����",
		m_LoginChattingClientSession->GetPlayerData()->GetPlayerID().c_str());

	//ä�� ��ã��, ���� ��Ŷ �۽�
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

	//�������� ä���� Ŭ���̾�Ʈ ���ǿ� �޽��� ����
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

	//ä�ο� �����ִٸ� ä���� �ɹ����� �۽�
	Channel* tempChannel = m_LoginChattingClientSession->GetPlayerData()->GetChannel();
	if (tempChannel)
	{
		SLogPrint(L"%s : ä�ο� �޽��� �۽�",
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
	
	//ä�ο��� Ŭ�󼼼��� ���������μ� ä�� ����
	EnterCriticalSection(&ChannelInOutCS);
	if (channel->DeleteClientSession(m_LoginChattingClientSession))
	{
		//ä�� ������ ����
		SLogPrintAtFile(L"%s : ä�� ������ ����", m_LoginChattingClientSession->GetPlayerData()->GetPlayerNickname().c_str());
		
		ansPacket.IsSuccess = true;
		ansPacket.ErrorNumber = ERROR_NONE;
		
		//ä�� ������ ���� ��Ŷ �۽�
		ansPacket.Encode(SendStream);
		SendPacketWithSendStream(E_PK_ANS_CHANNAL_OUT);

		//ä�� ������ ä�γ��� �ٸ� Ŭ�󿡰� �˸�
		SendChannelOutAnnounce();

		//�÷��̾��� ä�� ���� ������ �ٲ���
		m_LoginChattingClientSession->GetPlayerData()->SetChannel(nullptr);
		m_LoginChattingClientSession->GetPlayerData()->SetPlayerState(PLAYER_WAITING_CHANNEL);

		//ä�ο� �ƹ��� ���ٸ� ä�� ������
		if (channel->ChannelErase())
		{
			SAFE_DELETE(channel);
		}
		LeaveCriticalSection(&ChannelInOutCS);

		return;
	}
	LeaveCriticalSection(&ChannelInOutCS);

	SLogPrint(L"%s : ä�γ����� ����",
		m_LoginChattingClientSession->GetPlayerData()->GetPlayerID().c_str());

	//ä�� ������ ����
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

	//�ٸ� Ŭ���̾�Ʈ�� ��ε��ɽ��� ���ֱ� ���� ID �޾ƿ�
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

	//�ӵ� ����
	m_LoginChattingClientSession->GetPlayerData()->SetVelocity(10.0f);
	
	//�ش� Ŭ���̾�Ʈ�� player�� �Ʒ� �����͸� ������ �̵��Ѵٰ� ��ε��ɽ��� ��
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
	//ä�γ����ִ� Ŭ���̾�Ʈ�� ��Ŷ ����
	Channel* tempChannel = m_LoginChattingClientSession->GetPlayerData()->GetChannel();
	if (tempChannel)
	{
		tempChannel->SendPacketToChannelMember(SendStream);
	}

	//���� �ð� �־��
	m_LoginChattingClientSession->GetPlayerData()->m_DoStartTimePoint = TIMER->GetNowHighTimePoint();
}

void LoginChattingClientSessionParser::RecvMoveEnd(NetworkSystem::Packet * packetData)
{
	PK_SEND_MOVE_END* sendPacket = (PK_SEND_MOVE_END*)packetData;
	PK_RECV_MOVE_END recvPacket;

	//�ٸ� Ŭ���̾�Ʈ�� ��ε��ɽ��� ���ֱ� ���� ID �޾ƿ�
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

	//�ش� Ŭ���̾�Ʈ�� player�� �Ʒ� �����͸� ������ �̵��Ѵٰ� ��ε��ɽ��� ��
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
	//���� �ð� �־��
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

	//�ٸ� Ŭ���̾�Ʈ�� ��ε��ɽ��� ���ֱ� ���� ID �޾ƿ�
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
	
	//�ش� Ŭ���̾�Ʈ�� player�� �Ʒ� �����͸� ������ �̵��Ѵٰ� ��ε��ɽ��� ��
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

	//SLogPrint(L"%s�� �� : %d",
	//	m_LoginChattingClientSession->GetPlayerData()->GetPlayerID().c_str(),
	//	m_LoginChattingClientSession->GetPing());
 
	PK_SEND_SERVERTIME sendPacket;
	//�����ð� �۽�
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
	//�ٸ� Ŭ���̾�Ʈ�� ��ε��ɽ��� ���ֱ� ���� ID �޾ƿ�
	wstring ID = m_LoginChattingClientSession->GetPlayerData()->GetPlayerID();
	CHAR asiiID[32] = { 0, };
	Util::StrConvW2A((WCHAR*)ID.c_str(),
		asiiID, sizeof(asiiID));

	//�ش� Ŭ���̾�Ʈ�� player�� �Ʒ� �����͸� ������ �̵��Ѵٰ� ��ε��ɽ��� ��
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

	//�ٸ� Ŭ���̾�Ʈ�� ��ε��ɽ��� ���ֱ� ���� ID �޾ƿ�
	wstring ID = m_LoginChattingClientSession->GetPlayerData()->GetPlayerID();
	CHAR asiiID[32] = { 0, };
	Util::StrConvW2A((WCHAR*)ID.c_str(),
		asiiID, sizeof(asiiID));
	recvPacket.ID = asiiID;

	//�ش� Ŭ���̾�Ʈ�� player�� �Ʒ� �����͸� ������ �̵��Ѵٰ� ��ε��ɽ��� ��
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
