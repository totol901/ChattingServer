/******************************************************************************
파일명	: ClientSessionParser.h
목적		: 클라이언트 세션의 파싱 객체
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

			//스트림 관련 함수들
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