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

void ServerSessionParser::AnsLogin(T_PACKET * packet)
{
	bool IsSuccess = false;
	int errorNum = 0;
	char nickname[15] = { 0, };

	recvStream.set(packet->buff, PAKCET_BUFF_SIZE);
	recvStream.read(&IsSuccess, sizeof(IsSuccess));
	recvStream.read(&errorNum, sizeof(errorNum));
	recvStream.read(&nickname, sizeof(nickname));

	//�α��� ���� ����
	if (IsSuccess)
	{
		cout << "�α��� ����" << endl;
		cout << "�г��� : " << nickname << endl << endl;
		
		SCENEAMANGER->ChangeCurrentScene(WAITTING_CHANNEL);
		SCENEAMANGER->GetBeforeScene()->SignalEvent();
		return;
	}

	cout << "�α��� ���� " << endl;
	//cout << "���� ��ȣ :" << errorNum << endl;
	switch (errorNum)
	{
	case LOGIN_ERROR_WRONG_ID:
		cout << "!���̵� �߸���" << endl;
		break;
	case LOGIN_ERROR_WRONG_PW:
		cout << "!��й�ȣ�� �߸���" << endl;
		break;
	default:
		cout << "�α��� ������ȣ �̻�" << endl;
		ASSERT(false);
	}

	SCENEAMANGER->ChangeCurrentScene(LOGIN);
	SCENEAMANGER->GetBeforeScene()->SignalEvent();
}

void ServerSessionParser::AnsCreateId(T_PACKET * packet)
{
	bool IsSuccess = false;
	int errorNum = 0;
	char nickname[15] = { 0, };

	recvStream.set(packet->buff, PAKCET_BUFF_SIZE);
	recvStream.read(&IsSuccess, sizeof(IsSuccess));
	recvStream.read(&errorNum, sizeof(errorNum));

	if (IsSuccess)
	{
		cout << "���̵� ���� ����" << endl;

		SCENEAMANGER->ChangeCurrentScene(LOGIN);
		SCENEAMANGER->GetBeforeScene()->SignalEvent();
		return;
	}

	cout << "���̵� ���� ���� " << endl;
	cout << "���� ��ȣ :" << errorNum << endl;
	switch (errorNum)
	{
	case LOGIN_ERROR_CREATEID_ID_ALREADY_EXE:
		cout << "!ID �ߺ�" << endl;
		break;
	case LOGIN_ERROR_CREATEID_NICKNAME_ALREADY_EXE:
		cout << "!�г��� �ߺ�" << endl;
		break;
	default:
		cout << "���̵� ���� ������ȣ �̻�" << endl;
		ASSERT(false);
	}

	SCENEAMANGER->ChangeCurrentScene(LOGIN);
	SCENEAMANGER->GetBeforeScene()->SignalEvent();
}

void ServerSessionParser::AnsWaitingChannalEnter(T_PACKET * packet)
{
	bool IsSuccess = false;
	int errornum = 0;
	char channelList[64] = {0,};

	recvStream.set(packet->buff, PAKCET_BUFF_SIZE);
	recvStream.read(&IsSuccess, sizeof(IsSuccess));
	recvStream.read(&errornum, sizeof(errornum));
	recvStream.read(channelList, sizeof(channelList));

	cout << "ä�� ����Ʈ" << endl;
	if (IsSuccess)
	{
		if (*channelList != NULL)
		{
			cout << channelList;
		}
		else
		{
			cout << "������ ä���� ����." << endl;
		}
	}

	SCENEAMANGER->ChangeCurrentScene(WAITTING_CHANNEL);
	SCENEAMANGER->GetBeforeScene()->SignalEvent();
}

void ServerSessionParser::AnsWaitingChannelCreateChannel(T_PACKET * packet)
{
	bool IsSuccess = false;
	int errornum = 0;

	recvStream.set(packet->buff, PAKCET_BUFF_SIZE);
	recvStream.read(&IsSuccess, sizeof(IsSuccess));
	recvStream.read(&errornum, sizeof(errornum));

	if (IsSuccess)
	{
		cout << "ä�� ���� ����" << endl;

		SCENEAMANGER->ChangeCurrentScene(IN_CHANNEL);
		SCENEAMANGER->GetBeforeScene()->SignalEvent();
		return;
	}

	cout << "ä�� ���� ���� " << endl;
	cout << "���� ��ȣ :" << errornum << endl;

	SCENEAMANGER->ChangeCurrentScene(WAITTING_CHANNEL);
	SCENEAMANGER->GetBeforeScene()->SignalEvent();
}

void ServerSessionParser::AnsWaitingChannelJoin(T_PACKET * packet)
{
	bool IsSuccess = false;
	int errornum = 0;

	recvStream.set(packet->buff, PAKCET_BUFF_SIZE);
	recvStream.read(&IsSuccess, sizeof(IsSuccess));
	recvStream.read(&errornum, sizeof(errornum));

	if (IsSuccess)
	{
		cout << "ä�� ���� ����" << endl;

		SCENEAMANGER->ChangeCurrentScene(IN_CHANNEL);
		SCENEAMANGER->GetBeforeScene()->SignalEvent();
		return;
	}

	cout << "ä�� ���� ���� " << endl;
	cout << "���� ��ȣ :" << errornum << endl;

	SCENEAMANGER->ChangeCurrentScene(WAITTING_CHANNEL);
	SCENEAMANGER->GetBeforeScene()->SignalEvent();
}

void ServerSessionParser::RecvChannelMessage(T_PACKET * packet)
{
	char nickname[15] = {0,};
	char message[64] = {0,};

	recvStream.set(packet->buff, PAKCET_BUFF_SIZE);
	recvStream.read(nickname, sizeof(nickname));
	recvStream.read(message, sizeof(message));

	cout << nickname << " : " << message << endl;

	SCENEAMANGER->ChangeCurrentScene(IN_CHANNEL);
	SCENEAMANGER->GetBeforeScene()->SignalEvent();
}

void ServerSessionParser::AnsChannelOut(T_PACKET * packet)
{
	bool IsSuccess = false;
	int errornum = 0;

	recvStream.set(packet->buff, PAKCET_BUFF_SIZE);
	recvStream.read(&IsSuccess, sizeof(IsSuccess));
	recvStream.read(&errornum, sizeof(errornum));

	if (IsSuccess)
	{
		cout << "ä�� ������ ����" << endl;

		SCENEAMANGER->ChangeCurrentScene(WAITTING_CHANNEL);
		SCENEAMANGER->GetBeforeScene()->SignalEvent();
		return;
	}

	cout << "ä�� ������ ���� " << endl;
	cout << "���� ��ȣ :" << errornum << endl;

	SCENEAMANGER->ChangeCurrentScene(IN_CHANNEL);
	SCENEAMANGER->GetBeforeScene()->SignalEvent();
}

void ServerSessionParser::AnsExit(T_PACKET * packet)
{
	bool IsSuccess = false;
	int errornum = 0;

	recvStream.set(packet->buff, PAKCET_BUFF_SIZE);
	recvStream.read(&IsSuccess, sizeof(IsSuccess));
	recvStream.read(&errornum, sizeof(errornum));

	if (IsSuccess)
	{
		cout << "���� ���� ���� ����" << endl;
		exit(0);
		return;
	}

	cout << "���� ���� ���� ���� " << endl;
	cout << "���� ��ȣ :" << errornum << endl;
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
		break;
	default:
		ASSERT(false);
	}

	//if (!m_bConnected)
	//{
	//	return false;
	//}

	return true;
}

