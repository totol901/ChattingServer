#include "stdafx.h"
#include "ClientSessionParser.h"

ClientSessionParser::ClientSessionParser(ClientSession * clientSession)
	:m_ClientSession(clientSession)
{
}

ClientSessionParser::~ClientSessionParser()
{
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

void ClientSessionParser::ReqWatingChallnalEnter(T_PACKET * packet)
{
	bool isSucces = true;
	int errNum = 0;
	string vecChannel = CHANNELMANAGER->GetChannelList();

	SendStream.write(&isSucces, sizeof(isSucces));
	SendStream.write(&errNum, sizeof(errNum));
	//TODO : ���� ���� ������ �˻��ؾ���
	SendStream.write((void*)vecChannel.c_str(), 64);

	T_PACKET pk(PK_ANS_WAITINGCHANNAL_ENTER);
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
		ReqWatingChallnalEnter(packet);
		break;

	case PK_REQ_WAITINGCHANNAL_CHREAT_CHANNAL:
		break;

	case PK_REQ_WAITINGCHANNAL_CHANNAL_JOIN:
		break;

	case PK_REQ_CHANNAL_SEND_MESSAGE:
		break;

	case PK_REQ_CHANNAL_OUT:
		break;

	case PK_REQ_EXIT:
		
		return false;
	}

	//if (!m_bConnected)
	//{
	//	return false;
	//}

	return true;
}
