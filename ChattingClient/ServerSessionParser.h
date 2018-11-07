#pragma once

class IOCPServerSession;
class ServerSessionParser
{
private:
	IOCPServerSession * m_ClientSession;

	RecvStream recvStream;
	SendStream SendStream;

	void AnsLogin(T_PACKET* packet);
	void AnsCreateId(T_PACKET* packet);
	void AnsWaitingChannalEnter(T_PACKET* packet);
	void AnsWaitingChannelCreateChannel(T_PACKET* packet);
	void AnsWaitingChannelJoin(T_PACKET* packet);
	void RecvChannelMessage(T_PACKET* packet);
	void AnsChannelOut(T_PACKET* packet);
	void AnsExit(T_PACKET* packet);

public:
	ServerSessionParser(IOCPServerSession* ServerSession);
	~ServerSessionParser();

	bool PacketParsing(T_PACKET* const packet);
};

