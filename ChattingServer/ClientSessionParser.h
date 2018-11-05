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
	void ReqWatingChallnalEnter(T_PACKET* packet);

public:
	ClientSessionParser(ClientSession * clientSession);
	~ClientSessionParser();

	bool PacketParsing(T_PACKET* const packet);
};
