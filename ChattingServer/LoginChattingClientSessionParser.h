/******************************************************************************
파일명	: ClientSessionParser.h
목적		: 클라이언트 세션의 파싱 객체
******************************************************************************/
#pragma once
#include <thread>

class LoginChattingClientSession;
class LoginChattingClientSessionParser : public NetworkSystem::ClientSessionParser
{
private:
	LoginChattingClientSession* m_LoginChattingClientSession;
	
	//패킷 분석 함수들
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
