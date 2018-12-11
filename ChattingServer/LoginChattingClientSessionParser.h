/******************************************************************************
파일명	: ClientSessionParser.h
목적		: 클라이언트 세션의 파싱 객체
******************************************************************************/
#pragma once

class LoginChattingClientSession;
class LoginChattingClientSessionParser : public NetworkSystem::ClientSessionParser
{
private:
	LoginChattingClientSession* m_LoginChattingClientSession;
	
	//패킷 분석 함수들
	void ReqLogin(NetworkSystem::T_PACKET* packet);
	void ReqCreateID(NetworkSystem::T_PACKET * packet);
	void ReqWatingChannelEnter(NetworkSystem::T_PACKET* packet);
	void ReqWaitingChannelCreateChannel(NetworkSystem::T_PACKET* packet);
	void ReqWaitingChannelChannelJoin(NetworkSystem::T_PACKET* packet);
	void ReqChannelSendMessage(NetworkSystem::T_PACKET* packet);
	void ReqChannelOut(NetworkSystem::T_PACKET* packet);
	void ReqExit(NetworkSystem::T_PACKET* packet);
	void RecvMoveStart(NetworkSystem::T_PACKET* packet);
	void RecvMoveEnd(NetworkSystem::T_PACKET* packet);
	void RecvLocationRenewal(NetworkSystem::T_PACKET* packet);

public:
	LoginChattingClientSessionParser(ClientSession * clientSession);
	~LoginChattingClientSessionParser();
	
	virtual bool PacketParsing(T_PACKET* packet);
};
