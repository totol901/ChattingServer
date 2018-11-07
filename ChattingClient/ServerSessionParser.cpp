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

	//로그인 성공 유무
	if (IsSuccess)
	{
		cout << "로그인 성공" << endl;
		cout << "닉네임 : " << nickname << endl;
		return;
	}

	cout << "로그인 실패 " << endl;
	cout << "오류 번호 :" << errorNum << endl;
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
		cout << "아이디 생성 성공" << endl;
		return;
	}

	cout << "아이디 생성 실패 " << endl;
	cout << "오류 번호 :" << errorNum << endl;
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

	cout << "채널 리스트" << endl;
	if (IsSuccess)
	{
		if (*channelList != NULL)
		{
			cout << channelList;
		}
		else
		{
			cout << "생성된 채널이 없음." << endl;
		}

		return;
	}
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

