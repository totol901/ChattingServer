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
		cout << "�г��� : " << nickname << endl;
		return;
	}

	cout << "�α��� ���� " << endl;
	cout << "���� ��ȣ :" << errorNum << endl;
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
		return;
	}

	cout << "���̵� ���� ���� " << endl;
	cout << "���� ��ȣ :" << errorNum << endl;
}

void ServerSessionParser::AnsWaitingChannalEnter(T_PACKET * packet)
{
	bool IsSuccess = false;
	int errornum = 0;
	char channelList[64];

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
		return;
	}

	cout << "ä�� ���� ���� " << endl;
	cout << "���� ��ȣ :" << errornum << endl;
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
		return;
	}

	cout << "ä�� ���� ���� " << endl;
	cout << "���� ��ȣ :" << errornum << endl;
}

void ServerSessionParser::RecvChannelMessage(T_PACKET * packet)
{
	char nickname[15];
	char message[64];

	recvStream.set(packet->buff, PAKCET_BUFF_SIZE);
	recvStream.read(nickname, sizeof(nickname));
	recvStream.read(message, sizeof(message));

	cout << nickname << " : " << message << endl;
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
		return;
	}

	cout << "ä�� ������ ���� " << endl;
	cout << "���� ��ȣ :" << errornum << endl;
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
	}

	//if (!m_bConnected)
	//{
	//	return false;
	//}

	return true;
}

