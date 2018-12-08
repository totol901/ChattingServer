#include "stdafx.h"
#include "ClientSessionParser.h"
#include <atlstr.h>

namespace ServerEngine
{
	namespace NetworkSystem
	{
		static CRITICAL_SECTION ChannelInOutCS;

		ClientSessionParser::ClientSessionParser(ClientSession * clientSession)
			:m_ClientSession(clientSession)
		{
			InitializeCriticalSection(&RecvQueueCS);
			InitializeCriticalSection(&PacketParsingCS);

		}

		ClientSessionParser::~ClientSessionParser()
		{
			DeleteCriticalSection(&RecvQueueCS);
			DeleteCriticalSection(&PacketParsingCS);

			m_ClientSession = nullptr;
		}

		void ClientSessionParser::RecvQueuePkParsing()
		{
			T_PACKET * temp = PopQueueRecvPk();
			if (temp)
			{
				PacketParsing(temp);
			}
			SAFE_DELETE(temp);
		}

		bool ClientSessionParser::PacketParsing(T_PACKET* packet)
		{
			SLogPrintAtFile("PacketParsing override ¿¡·¯");
			return false;
		}

		void ClientSessionParser::SendPacketWithSendStream(E_PACKET_TYPE && type)
		{
			m_SendPk.Clear();
			m_SendPk.type = type;
			m_SendPk.SetStream(SendStream);

			m_ClientSession->SendPacket(m_SendPk);
		}

		void ClientSessionParser::SetRecvStream(T_PACKET * packet)
		{
			recvStream.clear();
			const size_t headSize = sizeof(packet->Size) + sizeof(packet->type);
			recvStream.set(packet->buff, packet->Size - headSize);
		}

		void ClientSessionParser::PushQueueRecvPk(T_PACKET * pPacket)
		{
			EnterCriticalSection(&RecvQueueCS);
			m_recvPkQueue.push(pPacket);
			LeaveCriticalSection(&RecvQueueCS);
		}

		T_PACKET * ClientSessionParser::PopQueueRecvPk()
		{
			EnterCriticalSection(&RecvQueueCS);
			if (m_recvPkQueue.size() == 0)
			{
				LeaveCriticalSection(&RecvQueueCS);
				return nullptr;
			}

			T_PACKET * temp = m_recvPkQueue.front();
			m_recvPkQueue.pop();

			LeaveCriticalSection(&RecvQueueCS);

			return temp;
		}
	}
}