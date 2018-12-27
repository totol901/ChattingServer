/******************************************************************************
���ϸ�	: ClientSessionParser.h
����		: Ŭ���̾�Ʈ ������ �Ľ� ��ü
******************************************************************************/
#pragma once

namespace ServerEngine
{
	namespace NetworkSystem
	{
		class ClientSession;
		class ClientSessionParser
		{
		protected:
			CRITICAL_SECTION RecvQueueCS;
			CRITICAL_SECTION PacketParsingCS;

			ClientSession * m_ClientSession;
			//Packet		m_SendPk;
			queue<Packet*> m_recvPkQueue;

			//RecvStream recvStream;
			SendStream SendStream;

			//��Ʈ�� ���� �Լ���
			void SendPacketWithSendStream(E_PACKET_TYPE&& type);
			//void SetRecvStream(Packet* packet);

		public:
			ClientSessionParser(ClientSession * clientSession);
			virtual ~ClientSessionParser();

			void PushQueueRecvPk(Packet* pPacketData);
			Packet* PopQueueRecvPk();

			void RecvHeatBeat(Packet * packetData);

			void RecvQueuePkParsing();

			virtual bool PacketParsing(Packet* packetData);

			const queue<Packet*>& GetRecvPkQueue() { return m_recvPkQueue; }
		};
	}
}