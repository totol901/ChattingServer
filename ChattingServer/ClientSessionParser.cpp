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

	//stream�� ���� ������ �־���
	recvStream.set(packet->buff, PAKCET_BUFF_SIZE);
	recvStream.read(id, sizeof(id));
	recvStream.read(pw, sizeof(pw));

	//id, pw ���� äũ
	if (DATABASE->CheckUserInfoQuery(id, pw))
	{
		//�α��� ����
		isSuccess = true;
		errorNum = 0;

		//stream�� ������ data ����
		string nickname = DATABASE->FindNickname(id);
		const char* str = nickname.c_str();
		SendStream.write(&isSuccess, sizeof(isSuccess));
		SendStream.write(&errorNum, sizeof(errorNum));
		SendStream.write((void*)str, nickname.size());

		SLogPrint("%s - �α��� ����", id);

		//�÷��̾� �����Ϳ� ���̵� �г��� ����
		m_ClientSession->GetPlayerData()->SetPlayerID(id);
		m_ClientSession->GetPlayerData()->SetPlayerNickname(str);
		m_ClientSession->GetPlayerData()->SetPlayerState(PLAYER_LOGIN);

		//�����ͺ��̽��� �α� ������
		DATABASE->InsertUserLogQuery(id, "�α��� �Ϸ�");
	}
	else
	{
		isSuccess = false;
		errorNum = 0;

		//stream�� ������ data ����
		SendStream.write(&isSuccess, sizeof(isSuccess));
		SendStream.write(&errorNum, sizeof(errorNum));

		SLogPrint("%s - �α��� ����", id);

		//�����ͺ��̽��� �α� ������
		DATABASE->InsertUserLogQuery(id, "�α��� �õ�");
	}

	//��Ŷ �۽�
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
		//���̵� ���� ����
		isSucces = true;
		errNum = 0;

		SendStream.write(&isSucces, sizeof(isSucces));
		SendStream.write(&errNum, sizeof(errNum));

		//�����ͺ��̽��� �α� ������
		DATABASE->InsertUserLogQuery(id, "���̵� ����");
	}
	else
	{
		isSucces = false;
		errNum = 0;

		SendStream.write(&isSucces, sizeof(isSucces));
		SendStream.write(&errNum, sizeof(errNum));

		SLogPrint("%s - ���̵� ���� ����", id);
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

	//�÷��̾� �����Ϳ� ���� ����
	m_ClientSession->GetPlayerData()->SetPlayerState(PLAYER_WAITING_CHANNEL);

	T_PACKET pk(PK_ANS_WAITINGCHANNAL_ENTER);
	pk.SetStream(SendStream);

	m_ClientSession->SendPacket(pk);
}

void ClientSessionParser::ReqWaitingChannelCreateChannel(T_PACKET * packet)
{
	//ä�� ����
	char channelName[15] = { 0, };
	recvStream.set(packet->buff, PAKCET_BUFF_SIZE);
	recvStream.read(channelName, sizeof(channelName));

	if (CHANNELMANAGER->MakeChannel(channelName))
	{
		//���� �Ϸ��ٴ� ��Ŷ ����
		bool isSucces = true;
		int errNum = 0;

		SendStream.write(&isSucces, sizeof(isSucces));
		SendStream.write(&errNum, sizeof(errNum));

		T_PACKET pk(PK_ANS_WAITINGCHANNAL_CHREAT_CHANNAL);
		pk.SetStream(SendStream);

		m_ClientSession->SendPacket(pk);

		//������ ä�ο� ����
		Channel* channel = CHANNELMANAGER->FindChannelByName(channelName);
		if (channel)
		{
			bool isSucces = true;
			int errNum = 0;
			//ä�ο� �����Ŵ
			channel->InsertClientSession(m_ClientSession);

			//�÷��̾� �����Ϳ� ������ ä�� ��������
			m_ClientSession->GetPlayerData()->SetChannel(channel);
			m_ClientSession->GetPlayerData()->SetPlayerState(PLAYER_IN_CHANNEL);

			//ä�� ���� ���� ��Ŷ �۽�
			SendStream.write(&isSucces, sizeof(isSucces));
			SendStream.write(&errNum, sizeof(errNum));
			pk.SetStream(SendStream);
			m_ClientSession->SendPacket(pk);
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
	int errNum = 0;

	SendStream.write(&isSucces, sizeof(isSucces));
	SendStream.write(&errNum, sizeof(errNum));

	T_PACKET pk(PK_ANS_WAITINGCHANNAL_CHREAT_CHANNAL);
	pk.SetStream(SendStream);

	m_ClientSession->SendPacket(pk);
}

void ClientSessionParser::ReqWaitingChannelChannelJoin(T_PACKET * packet)
{
	//�ش� ä�� ã��
	UINT channelNum = 0;
	recvStream.set(packet->buff, PAKCET_BUFF_SIZE);
	recvStream.read(&channelNum, sizeof(channelNum));

	//ä�� ã�� ������ ���� ��Ŷ �۽�
	T_PACKET pk(PK_ANS_WAITINGCHANNAL_CHANNAL_JOIN);

	EnterCriticalSection(&ChannelInOutCS);
	Channel* channel = CHANNELMANAGER->FindChannelByID(channelNum);
	if (channel)
	{
		bool isSucces = true;
		int errNum = 0;
		//ä�ο� �����Ŵ
		channel->InsertClientSession(m_ClientSession);

		//�÷��̾� �����Ϳ� ������ ä�� ��������
		m_ClientSession->GetPlayerData()->SetChannel(channel);
		m_ClientSession->GetPlayerData()->SetPlayerState(PLAYER_IN_CHANNEL);

		//ä�� ���� ���� ��Ŷ �۽�
		SendStream.write(&isSucces, sizeof(isSucces));
		SendStream.write(&errNum, sizeof(errNum));
		pk.SetStream(SendStream);
		m_ClientSession->SendPacket(pk);
		LeaveCriticalSection(&ChannelInOutCS);
		return;
	}
	LeaveCriticalSection(&ChannelInOutCS);

	//ä�� ��ã��, ���� ��Ŷ �۽�
	bool isSucces = false;
	int errNum = 0;
	SendStream.write(&isSucces, sizeof(isSucces));
	SendStream.write(&errNum, sizeof(errNum));
	pk.SetStream(SendStream);
	m_ClientSession->SendPacket(pk);
}

void ClientSessionParser::ReqChannelSendMessage(T_PACKET * packet)
{
	//�������� ä���� Ŭ���̾�Ʈ ���ǿ� �޽��� ����
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
	
	//ä�ο��� Ŭ�󼼼��� ���������μ� ä�� ����
	EnterCriticalSection(&ChannelInOutCS);
	if (channel->DeleteClientSession(m_ClientSession))
	{
		//ä�� ������ ����
		SLogPrintAtFile("%d : ä�� ������ ����", m_ClientSession->GetPlayerData()->GetPlayerNickname().c_str());
		
		bool isSucces = true;
		int errNum = 0;
		
		//ä�� ������ ���� ��Ŷ �۽�
		SendStream.write(&isSucces, sizeof(isSucces));
		SendStream.write(&errNum, sizeof(errNum));
		pk.SetStream(SendStream);
		m_ClientSession->SendPacket(pk);

		//ä�ο� �ƹ��� ���ٸ� ä�� ������
		if (channel->IsChannelEmpty())
		{
			CHANNELMANAGER->DeleteChannelByID(channel->GetID());
			SAFE_DELETE(channel);
		}
		LeaveCriticalSection(&ChannelInOutCS);
		return;
	}
	LeaveCriticalSection(&ChannelInOutCS);

	//ä�� ������ ����
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
