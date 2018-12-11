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

	//stream�� ���� ������ �־���
	const size_t headSize = sizeof(packet->Size) + sizeof(packet->type);
	recvStream.set(packet->buff, packet->Size - headSize);

	recvStream.wStringread(id);
	recvStream.wStringread(pw);

	//id, pw ���� äũ
	if (DATABASE->CheckUserInfoQuery(id, pw))
	{
		//�α��� ������ Ȯ��
		if (CLIENTSESSIONMANAGER->FindClientSessionID(
			id) != nullptr)
		{
			//�α��� �̹� ��
			isSuccess = false;
			errorNum = LOGIN_ERROR_ALREADY_LOGIN;

			//stream�� ������ data ����
			SendStream.write(&isSuccess, sizeof(isSuccess));
			SendStream.write(&errorNum, sizeof(errorNum));

			SLogPrint(L"%s - �α��� ����", id.c_str());

			//�����ͺ��̽��� �α� ������
			DATABASE->InsertUserLogQuery(id, L"�α��� ����");
		}
		else
		{
			//�α��� ����
			isSuccess = true;
			errorNum = ERROR_NONE;

			//stream�� ������ data ����
			wstring nickname = DATABASE->FindNickname(id);
			SendStream.write(&isSuccess, sizeof(isSuccess));
			SendStream.write(&errorNum, sizeof(errorNum));
			SendStream.write(nickname);

			SLogPrint(L"%s - �α��� ����", id.c_str());

			//�÷��̾� �����Ϳ� ���̵� �г��� ����
			m_LoginChattingClientSession->GetPlayerData()->SetPlayerID(id);
			m_LoginChattingClientSession->GetPlayerData()->SetPlayerNickname(nickname);
			m_LoginChattingClientSession->GetPlayerData()->SetPlayerState(PLAYER_LOGIN);

			//�����ͺ��̽��� �α� ������
			DATABASE->InsertUserLogQuery(id, L"�α��� �Ϸ�");

			//�α��ε� ���� Ǯ�� ����
			CLIENTSESSIONMANAGER->AddClientSessionID(id, m_LoginChattingClientSession);
		}
	}
	else
	{
		isSuccess = false;
		errorNum = LOGIN_ERROR_WRONG;

		//stream�� ������ data ����
		SendStream.write(&isSuccess, sizeof(isSuccess));
		SendStream.write(&errorNum, sizeof(errorNum));

		SLogPrint(L"%s - �α��� ����", id.c_str());

		//�����ͺ��̽��� �α� ������
		DATABASE->InsertUserLogQuery(id, L"�α��� ����");
	}

	//��Ŷ �۽�
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
		//���̵� ���� ����
		isSucces = true;
		errNum = ERROR_NONE;

		SendStream.write(&isSucces, sizeof(isSucces));
		SendStream.write(&errNum, sizeof(errNum));

		//�����ͺ��̽��� �α� ������
		DATABASE->InsertUserLogQuery(id, L"���̵� ����");
	}
	else
	{
		isSucces = false;
		errNum = LOGIN_ERROR_CREATEID_ID_ALREADY_EXE;

		SendStream.write(&isSucces, sizeof(isSucces));
		SendStream.write(&errNum, sizeof(errNum));

		SLogPrint(L"%s - ���̵� ���� ����", id);
	}

	m_SendPk.Clear();
	m_SendPk.type = PK_ANS_CREATE_ID;
	m_SendPk.SetStream(SendStream);
	m_ClientSession->SendPacket(m_SendPk);
}

void LoginChattingClientSessionParser::ReqWatingChannelEnter(T_PACKET * packet)
{
	SLogPrint(L"%s : ä�� ����Ʈ ��û",
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

	//�÷��̾� �����Ϳ� ���� ����
	m_LoginChattingClientSession->GetPlayerData()->SetPlayerState(PLAYER_WAITING_CHANNEL);

	m_SendPk.Clear();
	m_SendPk.type = PK_ANS_WAITINGCHANNAL_ENTER;
	m_SendPk.SetStream(SendStream);
	m_LoginChattingClientSession->SendPacket(m_SendPk);
}

void LoginChattingClientSessionParser::ReqWaitingChannelCreateChannel(T_PACKET * packet)
{
	SendStream.clear();
	//ä�� ����
	wstring channelName;
	SetRecvStream(packet);
	recvStream.wStringread(channelName);

	if (CHANNELMANAGER->MakeChannelWithChannelName(channelName))
	{
		SLogPrintAtFile(L"%s : ä�� ���� ����", m_LoginChattingClientSession->GetPlayerData()->GetPlayerNickname().c_str());

		//���� �Ϸ��ٴ� ��Ŷ ����
		bool isSucces = true;
		int errNum = ERROR_NONE;

		SendStream.write(&isSucces, sizeof(isSucces));
		SendStream.write(&errNum, sizeof(errNum));
		SendPacketWithSendStream(PK_ANS_WAITINGCHANNAL_CHREAT_CHANNAL);

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
	bool isSucces = false;
	int errNum = CREATE_CHANNEL_ALREADY_EXE;

	SendStream.write(&isSucces, sizeof(isSucces));
	SendStream.write(&errNum, sizeof(errNum));
	SendPacketWithSendStream(PK_ANS_WAITINGCHANNAL_CHREAT_CHANNAL);
}

void LoginChattingClientSessionParser::ReqWaitingChannelChannelJoin(T_PACKET * packet)
{
	SendStream.clear();
	//�ش� ä�� ã��
	UINT channelNum = 0;
	SetRecvStream(packet);
	recvStream.read(&channelNum, sizeof(channelNum));

	//ä�� ã�� ������ ���� ��Ŷ �۽�
	EnterCriticalSection(&ChannelInOutCS);
	Channel* channel = CHANNELMANAGER->FindChannelByID(channelNum);
	if (channel)
	{
		bool isSucces = true;
		int errNum = ERROR_NONE;
		//ä�ο� �����Ŵ
		channel->InsertClientSession(m_ClientSession);

		//�÷��̾� �����Ϳ� ������ ä�� ��������
		m_LoginChattingClientSession->GetPlayerData()->SetChannel(channel);
		m_LoginChattingClientSession->GetPlayerData()->SetPlayerState(PLAYER_IN_CHANNEL);

		SLogPrint(L"%s : %s, ä�� ���� ����",
			m_LoginChattingClientSession->GetPlayerData()->GetPlayerID().c_str(),
			channel->GetName().c_str());

		//ä�� ���� ���� ��Ŷ �۽�
		SendStream.write(&isSucces, sizeof(isSucces));
		SendStream.write(&errNum, sizeof(errNum));
		SendPacketWithSendStream(PK_ANS_WAITINGCHANNAL_CHANNAL_JOIN);

		LeaveCriticalSection(&ChannelInOutCS);
		return;
	}
	LeaveCriticalSection(&ChannelInOutCS);

	SLogPrint(L"%s : !ä�� ���� ����",
		m_LoginChattingClientSession->GetPlayerData()->GetPlayerID().c_str());

	//ä�� ��ã��, ���� ��Ŷ �۽�
	bool isSucces = false;
	int errNum = ENTER_CHANNEL_CANT_FIND;
	SendStream.write(&isSucces, sizeof(isSucces));
	SendStream.write(&errNum, sizeof(errNum));
	SendPacketWithSendStream(PK_ANS_WAITINGCHANNAL_CHANNAL_JOIN);
}

void LoginChattingClientSessionParser::ReqChannelSendMessage(T_PACKET * packet)
{
	SendStream.clear();
	//�������� ä���� Ŭ���̾�Ʈ ���ǿ� �޽��� ����
	wstring message;
	SetRecvStream(packet);
	recvStream.wStringread(message);

	wstring nickname = m_LoginChattingClientSession->GetPlayerData()->GetPlayerNickname();
	
	int errNum = ERROR_NONE;
	SendStream.write(nickname);
	SendStream.write(message);

	T_PACKET pk(PK_RECV_CHANNAL_MESSAGE);
	pk.SetStream(SendStream);

	//ä�ο� �����ִٸ� ä���� �ɹ����� �۽�
	Channel* tempChannel = m_LoginChattingClientSession->GetPlayerData()->GetChannel();
	if (tempChannel)
	{
		SLogPrint(L"%s : ä�ο� �޽��� �۽�",
			m_LoginChattingClientSession->GetPlayerData()->GetPlayerID().c_str());
		tempChannel->SendPacketToChannelMember(pk);
	}
}

void LoginChattingClientSessionParser::ReqChannelOut(T_PACKET * packet)
{
	SendStream.clear();
	Channel* channel = m_LoginChattingClientSession->GetPlayerData()->GetChannel();
	
	//ä�ο��� Ŭ�󼼼��� ���������μ� ä�� ����
	EnterCriticalSection(&ChannelInOutCS);
	if (channel->DeleteClientSession(m_ClientSession))
	{
		//ä�� ������ ����
		SLogPrintAtFile(L"%s : ä�� ������ ����", m_LoginChattingClientSession->GetPlayerData()->GetPlayerNickname().c_str());
		
		bool isSucces = true;
		int errNum = ERROR_NONE;
		
		//ä�� ������ ���� ��Ŷ �۽�
		SendStream.write(&isSucces, sizeof(isSucces));
		SendStream.write(&errNum, sizeof(errNum));
		SendPacketWithSendStream(PK_ANS_CHANNAL_OUT);

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

	//Ŭ���̾�Ʈ�� ���� ��ġ ���� �ӵ��� �޾ƿ���
	const size_t headSize = sizeof(packet->Size) + sizeof(packet->type);
	recvStream.set(packet->buff, packet->Size - headSize);
	recvStream.read(&LocationX, sizeof(float));
	recvStream.read(&LocationY, sizeof(float));
	recvStream.read(&DirectionX, sizeof(float));
	recvStream.read(&DirectionY, sizeof(float));
	recvStream.read(&Velocity, sizeof(float));

	//�ٸ� Ŭ���̾�Ʈ�� ��ε��ɽ��� ���ֱ� ���� ID �޾ƿ�
	wstring ID = m_LoginChattingClientSession->GetPlayerData()->GetPlayerID();
	m_LoginChattingClientSession->GetPlayerData()->SetLocation(LocationX, LocationY);
	m_LoginChattingClientSession->GetPlayerData()->SetAcceleration(1.3f);

	SLogPrint(L"%s : MoveStart",
		m_LoginChattingClientSession->GetPlayerData()->GetPlayerID().c_str());

	//�ش� Ŭ���̾�Ʈ�� player�� �Ʒ� �����͸� ������ �̵��Ѵٰ� ��ε��ɽ��� ��
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

	//Ŭ���̾�Ʈ�� ���� ��ġ ���� �ӵ��� �޾ƿ���
	const size_t headSize = sizeof(packet->Size) + sizeof(packet->type);
	recvStream.set(packet->buff, packet->Size - headSize);
	recvStream.read(&LocationX, sizeof(float));
	recvStream.read(&LocationY, sizeof(float));
	recvStream.read(&DirectionX, sizeof(float));
	recvStream.read(&DirectionY, sizeof(float));
	recvStream.read(&Velocity, sizeof(float));

	SLogPrint(L"%s : MoveEnd",
		m_LoginChattingClientSession->GetPlayerData()->GetPlayerID().c_str());

	//�ٸ� Ŭ���̾�Ʈ�� ��ε��ɽ��� ���ֱ� ���� ID �޾ƿ�
	wstring ID = m_LoginChattingClientSession->GetPlayerData()->GetPlayerID();
	m_LoginChattingClientSession->GetPlayerData()->SetLocation(LocationX, LocationY);
	m_LoginChattingClientSession->GetPlayerData()->SetAcceleration(-1.3f);

	//�ش� Ŭ���̾�Ʈ�� player�� �Ʒ� �����͸� ������ �̵��Ѵٰ� ��ε��ɽ��� ��
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

	//Ŭ���̾�Ʈ�� ���� ��ġ ���� �ӵ��� �޾ƿ���
	const size_t headSize = sizeof(packet->Size) + sizeof(packet->type);
	recvStream.set(packet->buff, packet->Size - headSize);
	recvStream.read(&LocationX, sizeof(float));
	recvStream.read(&LocationY, sizeof(float));

	SLogPrint(L"%s : LocationRenewal",
		m_LoginChattingClientSession->GetPlayerData()->GetPlayerID().c_str());

	//�ٸ� Ŭ���̾�Ʈ�� ��ε��ɽ��� ���ֱ� ���� ID �޾ƿ�
	wstring ID = m_LoginChattingClientSession->GetPlayerData()->GetPlayerID();
	m_LoginChattingClientSession->GetPlayerData()->SetLocation(LocationX, LocationY);
	
	//�ش� Ŭ���̾�Ʈ�� player�� �Ʒ� �����͸� ������ �̵��Ѵٰ� ��ε��ɽ��� ��
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
