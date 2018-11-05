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

	//stream에 받은 데이터 넣어줌
	recvStream.set(packet->buff, PAKCET_BUFF_SIZE);
	recvStream.read(id, sizeof(id));
	recvStream.read(pw, sizeof(pw));

	//id, pw 쿼리 채크
	if (DATABASE->CheckQuery(id, pw))
	{
		isSuccess = true;
		errorNum = 0;

		//stream에 보내줄 data 써줌
		string nickname = DATABASE->FindNickname(id);
		const char* str = nickname.c_str();
		SendStream.write(&isSuccess, sizeof(isSuccess));
		SendStream.write(&errorNum, sizeof(errorNum));
		SendStream.write((void*)str, nickname.size());
	}
	else
	{
		isSuccess = false;
		errorNum = 0;

		//stream에 보내줄 data 써줌
		SendStream.write(&isSuccess, sizeof(isSuccess));
		SendStream.write(&errorNum, sizeof(errorNum));
	}

	//패킷 송신
	pk.SetStream(SendStream);
	m_ClientSession->SendPacket(pk);
}

void ClientSessionParser::ReqCreateID(T_PACKET * packet)
{
	recvStream.clear();
	SendStream.clear();
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

	if (DATABASE->InsertQuery(id, pw, nickname))
	{
		isSucces = true;
		errNum = 0;

		SendStream.write(&isSucces, sizeof(isSucces));
		SendStream.write(&errNum, sizeof(errNum));
	}
	else
	{
		isSucces = false;
		errNum = 0;

		SendStream.write(&isSucces, sizeof(isSucces));
		SendStream.write(&errNum, sizeof(errNum));
	}

	m_ClientSession->SendPacket(pk);
}

bool ClientSessionParser::PacketParsing(T_PACKET * const packet)
{
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
