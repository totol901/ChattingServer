#include "stdafx.h"
#include "ClientSessionParser.h"
#include <atlstr.h>

static CRITICAL_SECTION ChannelInOutCS;

ClientSessionParser::ClientSessionParser(ClientSession * clientSession)
	:m_ClientSession(clientSession)
{
	InitializeCriticalSection(&RecvQueueCS);
	InitializeCriticalSection(&PacketParsingCS);
	InitializeCriticalSection(&ChannelInOutCS);
}

ClientSessionParser::~ClientSessionParser()
{
	DeleteCriticalSection(&RecvQueueCS);
	DeleteCriticalSection(&PacketParsingCS);
	DeleteCriticalSection(&ChannelInOutCS);
	m_ClientSession = nullptr;
}

void ClientSessionParser::RecvQueuePkParsing()
{
	T_PACKET * temp = PopQueueRecvPk();
	if (temp)
	{
		PacketParsing(temp);
	}
	SAFE_DELETE(temp);
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
			m_ClientSession->GetPlayerData()->SetPlayerID(id);
			m_ClientSession->GetPlayerData()->SetPlayerNickname(nickname);
			m_ClientSession->GetPlayerData()->SetPlayerState(PLAYER_LOGIN);

			//�����ͺ��̽��� �α� ������
			DATABASE->InsertUserLogQuery(id, L"�α��� �Ϸ�");

			//�α��ε� ���� Ǯ�� ����
			CLIENTSESSIONMANAGER->AddClientSessionID(id, m_ClientSession);
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

void ClientSessionParser::ReqCreateID(T_PACKET * packet)
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

void ClientSessionParser::ReqWatingChannelEnter(T_PACKET * packet)
{
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
	m_ClientSession->GetPlayerData()->SetPlayerState(PLAYER_WAITING_CHANNEL);

	m_SendPk.Clear();
	m_SendPk.type = PK_ANS_WAITINGCHANNAL_ENTER;
	m_SendPk.SetStream(SendStream);
	m_ClientSession->SendPacket(m_SendPk);
}

void ClientSessionParser::ReqWaitingChannelCreateChannel(T_PACKET * packet)
{
	SendStream.clear();
	//ä�� ����
	wstring channelName;
	SetRecvStream(packet);
	recvStream.wStringread(channelName);

	if (CHANNELMANAGER->MakeChannelWithChannelName(channelName))
	{
		SLogPrintAtFile(L"%s : ä�� ���� ����", m_ClientSession->GetPlayerData()->GetPlayerNickname().c_str());

		//���� �Ϸ��ٴ� ��Ŷ ����
		bool isSucces = true;
		int errNum = ERROR_NONE;

		SendStream.write(&isSucces, sizeof(isSucces));
		SendStream.write(&errNum, sizeof(errNum));
		SendPacketWithSendStream(PK_ANS_WAITINGCHANNAL_CHREAT_CHANNAL);

		//������ ä�ο� ����
		if (CHANNELMANAGER->ClientJoinTheChannel(m_ClientSession, channelName))
		{
			SLogPrint("ä�� ���� ����");
		}
		else
		{
			SLogPrintAtFile("!ä�� ������ ���� ����");
			ASSERT(FALSE);
		}

		return;
	}
	
	//���� ����
	bool isSucces = false;
	int errNum = CREATE_CHANNEL_ALREADY_EXE;

	SendStream.write(&isSucces, sizeof(isSucces));
	SendStream.write(&errNum, sizeof(errNum));
	SendPacketWithSendStream(PK_ANS_WAITINGCHANNAL_CHREAT_CHANNAL);
}

void ClientSessionParser::ReqWaitingChannelChannelJoin(T_PACKET * packet)
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
		m_ClientSession->GetPlayerData()->SetChannel(channel);
		m_ClientSession->GetPlayerData()->SetPlayerState(PLAYER_IN_CHANNEL);

		//ä�� ���� ���� ��Ŷ �۽�
		SendStream.write(&isSucces, sizeof(isSucces));
		SendStream.write(&errNum, sizeof(errNum));
		SendPacketWithSendStream(PK_ANS_WAITINGCHANNAL_CHANNAL_JOIN);

		LeaveCriticalSection(&ChannelInOutCS);
		return;
	}
	LeaveCriticalSection(&ChannelInOutCS);

	//ä�� ��ã��, ���� ��Ŷ �۽�
	bool isSucces = false;
	int errNum = ENTER_CHANNEL_CANT_FIND;
	SendStream.write(&isSucces, sizeof(isSucces));
	SendStream.write(&errNum, sizeof(errNum));
	SendPacketWithSendStream(PK_ANS_WAITINGCHANNAL_CHANNAL_JOIN);
}

void ClientSessionParser::ReqChannelSendMessage(T_PACKET * packet)
{
	SendStream.clear();
	//�������� ä���� Ŭ���̾�Ʈ ���ǿ� �޽��� ����
	wstring message;
	SetRecvStream(packet);
	recvStream.wStringread(message);

	wstring nickname = m_ClientSession->GetPlayerData()->GetPlayerNickname();
	
	int errNum = ERROR_NONE;
	SendStream.write(nickname);
	SendStream.write(message);

	T_PACKET pk(PK_RECV_CHANNAL_MESSAGE);
	pk.SetStream(SendStream);

	//ä�ο� �����ִٸ� ä���� �ɹ����� �۽�
	Channel* tempChannel = m_ClientSession->GetPlayerData()->GetChannel();
	if (tempChannel)
	{
		tempChannel->SendPacketToChannelMember(pk);
	}
}

void ClientSessionParser::ReqChannelOut(T_PACKET * packet)
{
	SendStream.clear();
	Channel* channel = m_ClientSession->GetPlayerData()->GetChannel();
	
	//ä�ο��� Ŭ�󼼼��� ���������μ� ä�� ����
	EnterCriticalSection(&ChannelInOutCS);
	if (channel->DeleteClientSession(m_ClientSession))
	{
		//ä�� ������ ����
		SLogPrintAtFile(L"%s : ä�� ������ ����", m_ClientSession->GetPlayerData()->GetPlayerNickname().c_str());
		
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

	//ä�� ������ ����
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

	default:
		ASSERT(false);
	}
	LeaveCriticalSection(&PacketParsingCS);

	return true;
}

void ClientSessionParser::PushQueueRecvPk(T_PACKET * pPacket)
{
	EnterCriticalSection(&RecvQueueCS);
	m_recvPkQueue.push(pPacket);
	LeaveCriticalSection(&RecvQueueCS);
}

T_PACKET * ClientSessionParser::PopQueueRecvPk()
{
	EnterCriticalSection(&RecvQueueCS);
	if (m_recvPkQueue.size() == 0)
	{
		LeaveCriticalSection(&RecvQueueCS);
		return nullptr;
	}

	T_PACKET * temp = m_recvPkQueue.front();
	m_recvPkQueue.pop();

	LeaveCriticalSection(&RecvQueueCS);

	return temp;
}
