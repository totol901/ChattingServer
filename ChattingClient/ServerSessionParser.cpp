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
		break;

	case PK_ANS_WAITINGCHANNAL_CHREAT_CHANNAL:
		break;

	case PK_ANS_WAITINGCHANNAL_CHANNAL_JOIN:
		break;

	case PK_RECV_CHANNAL_MESSAGE:
		break;

	case PK_ANS_CHANNAL_OUT:
		break;

	case PK_ANS_EXIT:
		break;
	}

	//if (!m_bConnected)
	//{
	//	return false;
	//}

	return true;
}

