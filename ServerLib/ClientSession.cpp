#include "stdafx.h"
#include "ClientSession.h"

namespace ServerEngine
{
	namespace NetworkSystem
	{
		BOOL ClientSession::m_AllocatorOn = FALSE;

		ClientSession::ClientSession()
			:Session()
		{
			m_Type = SESSION_TYPE_CLIENT;
		}

		ClientSession::~ClientSession()
		{
		}

		//void ClientSession::Release()
		//{
		//	//채널에 있을수 있으니 있다면 빠져나옴
		//	//if (m_PlayerData.GetChannel())
		//	//{
		//	//	m_PlayerData.GetChannel()->DeleteClientSession(this);
		//	//	m_PlayerData.GetChannel()->ChannelErase();
		//	//}
		//}

		IOData * const ClientSession::GetptIOData(const int & type)
		{
			switch (type)
			{
			case IO_READ:
				return &m_arrIOData[IO_READ];

			case IO_WRITE:
				return &m_arrIOData[IO_WRITE];

			default:
				return nullptr;
			}
		}

		HRESULT ClientSession::Init()
		{
			return S_OK;
		}

		void ClientSession::Release()
		{
		}

		const bool ClientSession::PacketParsing(Packet * const packet)
		{
			return false;
		}
	}
}
