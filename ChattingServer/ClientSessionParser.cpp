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

void ClientSessionParser::ReqWatingChallnalEnter(T_PACKET * packet)
{
	bool isSucces = true;
	int errNum = 0;
	string vecChannel = CHANNELMANAGER->GetChannelList();

	SendStream.write(&isSucces, sizeof(isSucces));
	SendStream.write(&errNum, sizeof(errNum));
	//TODO : 벡터 재대로 써지나 검사해야함
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
