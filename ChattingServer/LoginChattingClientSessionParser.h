/******************************************************************************
���ϸ�	: ClientSessionParser.h
����		: Ŭ���̾�Ʈ ������ �Ľ� ��ü
******************************************************************************/
#pragma once
#include <thread>

class LoginChattingClientSession;
class LoginChattingClientSessionParser : public NetworkSystem::ClientSessionParser
{
private:
	LoginChattingClientSession* m_LoginChattingClientSession;
	
	//��Ŷ �м� �Լ���
	void ReqLogin(NetworkSystem::Packet* packetData);
	void ReqCreateID(NetworkSystem::Packet * packetData);
	void ReqWatingChannelEnter(NetworkSystem::Packet* packetData);
	void ReqWaitingChannelCreateChannel(NetworkSystem::Packet* packetData);
	void ReqWaitingChannelChannelJoin(NetworkSystem::Packet* packetData);
	void ReqChannelSendMessage(NetworkSystem::Packet* packetData);
	void ReqChannelOut(NetworkSystem::Packet* packetData);
	void ReqExit(NetworkSystem::Packet* packetData);
	void RecvMoveStart(NetworkSystem::Packet* packetData);
	void RecvMoveEnd(NetworkSystem::Packet* packetData);
	void RecvLocationRenewal(NetworkSystem::Packet* packetData);
	void RecvServerTime(NetworkSystem::Packet* packetData);
	
	void SendChannelJoinAnnounce();
	
	void SendChannelData();

public:
	LoginChattingClientSessionParser(ClientSession * clientSession);
	~LoginChattingClientSessionParser();
	
	void SendChannelOutAnnounce();

	virtual bool PacketParsing(Packet* packetData);
};
