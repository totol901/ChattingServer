/******************************************************************************
파일명	: ClientSessionParser.h
목적		: 클라이언트 세션의 파싱 객체
******************************************************************************/
#pragma once

class ClientSession;
class ClientSessionParser
{
private:
	CRITICAL_SECTION RecvQueueCS;
	CRITICAL_SECTION PacketParsingCS;
	ClientSession * m_ClientSession;
	T_PACKET		m_SendPk;
	queue<T_PACKET*> m_recvPkQueue;

	RecvStream recvStream;
	SendStream SendStream;

	//스트림 관련 함수들
	void SendPacketWithSendStream(E_PACKET_TYPE&& type);
	void SetRecvStream(T_PACKET* packet);

	//패킷 분석 함수들
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

	void PushQueueRecvPk(T_PACKET* pPacket);
	T_PACKET* PopQueueRecvPk();

	void RecvQueuePkParsing();

	bool PacketParsing(T_PACKET* const packet);
	const queue<T_PACKET*>& GetRecvPQueue() { return m_recvPkQueue; }
};
