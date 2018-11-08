/******************************************************************************
파일명	: ClientSessionParser.h
목적		: 클라이언트 세션의 파싱 객체
******************************************************************************/
#pragma once

class ClientSession;
class ClientSessionParser
{
private:
	ClientSession * m_ClientSession;
	RecvStream recvStream;
	SendStream SendStream;

	void ReqLogin(T_PACKET* packet);
	void ReqCreateID(T_PACKET * packet);
	void ReqWatingChannelEnter(T_PACKET* packet);
	void ReqWaitingChannelCreateChannel(T_PACKET* packet);
	void ReqWaitingChannelChannelJoin(T_PACKET* packet);
	void ReqChannelSendMessage(T_PACKET* packet);
	void ReqChannelOut(T_PACKET* packet);
	void ReqExit(T_PACKET* packet);

public:
	ClientSessionParser(ClientSession * clientSession);
	~ClientSessionParser();

	bool PacketParsing(T_PACKET* const packet);
};
