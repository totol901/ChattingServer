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
			T_PACKET		m_SendPk;
			queue<T_PACKET*> m_recvPkQueue;

			RecvStream recvStream;
			SendStream SendStream;

			//스트림 관련 함수들
			void SendPacketWithSendStream(E_PACKET_TYPE&& type);
			void SetRecvStream(T_PACKET* packet);

		public:
			ClientSessionParser(ClientSession * clientSession);
			virtual ~ClientSessionParser();

			void PushQueueRecvPk(T_PACKET* pPacket);
			T_PACKET* PopQueueRecvPk();

			void RecvQueuePkParsing();

			virtual bool PacketParsing(T_PACKET* packet);

			const queue<T_PACKET*>& GetRecvPQueue() { return m_recvPkQueue; }
		};
	}
}