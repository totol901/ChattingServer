/******************************************************************************
���ϸ�	: ClientSessionParser.h
����		: Ŭ���̾�Ʈ ������ �Ľ� ��ü
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

	//��Ʈ�� ���� �Լ���
	void SendPacketWithSendStream(E_PACKET_TYPE&& type);
	void SetRecvStream(T_PACKET* packet);

	//��Ŷ �м� �Լ���
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
