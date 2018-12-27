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
			//¾ø¾Ö±â
			EnterCriticalSection(&RecvQueueCS);
			EnterCriticalSection(&PacketParsingCS);
			while (m_recvPkQueue.size() != 0)
			{
				Packet* temp = m_recvPkQueue.front();
				SAFE_DELETE(temp);
				m_recvPkQueue.pop();
			}
			LeaveCriticalSection(&PacketParsingCS);
			LeaveCriticalSection(&RecvQueueCS);

			DeleteCriticalSection(&RecvQueueCS);
			DeleteCriticalSection(&PacketParsingCS);

			m_ClientSession = nullptr;
		}

		void ClientSessionParser::RecvQueuePkParsing()
		{
			Packet * temp = PopQueueRecvPk();
			if (temp)
			{
				PacketParsing(temp);
			}
			SAFE_DELETE(temp);
		}

		bool ClientSessionParser::PacketParsing(Packet* packetData)
		{
			switch (packetData->Type())
			{
			case E_PK_RECV_HARTBEAT:
				RecvHeatBeat(packetData);
				return true;
			}

			return false;
		}

		void ClientSessionParser::SendPacketWithSendStream(E_PACKET_TYPE && type)
		{
			Packet* packet = PACKETMAKER->GetPacket(type);
			packet->Encode(SendStream);
			SAFE_DELETE(packet);

			m_ClientSession->SendPacket(SendStream);
		}

		//void ClientSessionParser::SetRecvStream(Packet * packet)
		//{
		//	recvStream.clear();
		//	packet->Decode(recvStream);
		//}

		void ClientSessionParser::PushQueueRecvPk(Packet * pPacketData)
		{
			EnterCriticalSection(&RecvQueueCS);
			if (m_recvPkQueue.size() != 0)
			{
				if (m_recvPkQueue.back()->Type() != E_PK_SEND_MOVE_END)
				{
					m_recvPkQueue.push(pPacketData);
				}
			}
			else
			{
				if (m_ClientSession != nullptr)
				{
					m_recvPkQueue.push(pPacketData);
				}
			}
			LeaveCriticalSection(&RecvQueueCS);
		}

		Packet * ClientSessionParser::PopQueueRecvPk()
		{
			EnterCriticalSection(&RecvQueueCS);
			if (m_recvPkQueue.size() == 0)
			{
				LeaveCriticalSection(&RecvQueueCS);
				return nullptr;
			}

			Packet * temp = m_recvPkQueue.front();
			m_recvPkQueue.pop();

			LeaveCriticalSection(&RecvQueueCS);

			return temp;
		}

		void ClientSessionParser::RecvHeatBeat(Packet * packetData)
		{
			m_ClientSession->RecvHeartBeat();
		}
	}
}