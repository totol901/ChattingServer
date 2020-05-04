#include "stdafx.h"
#include "LoginProcess.h"

LoginProcess::LoginProcess()
{
}

LoginProcess::~LoginProcess()
{
}

void LoginProcess::registSubPacketFunc()
{
	m_runFuncTable.insert(
		make_pair(NetworkSystem::E_PACKET_TYPE::E_PK_REQ_LOGIN,
			[](NetworkSystem::Session* pSession, NetworkSystem::Packet* rowPacket)
			{
				NetworkSystem::PK_REQ_LOGIN* packet = (NetworkSystem::PK_REQ_LOGIN*)rowPacket;
				SLogPrintAtFile(L"* id/pw packet: %S / %S", packet->ID.c_str(), packet->PW.c_str());

				NetworkSystem::PK_I_DB_REQ_ID_PW DBPacket;
				DBPacket.ClientID = pSession->GetId();
				DBPacket.ID = packet->ID;
				DBPacket.PW = packet->PW;

				//DBAgent로 패킷 송신
				NetworkSystem::Terminal *terminal = TERMINALMANAGER->FindTerminal(L"DBAgent");
				terminal->SendPacket(&DBPacket);
			}
	));

	m_runFuncTable.insert(
		make_pair(NetworkSystem::E_PACKET_TYPE::E_PK_I_DB_ANS_ID_PW,
			[](NetworkSystem::Session* pSession, NetworkSystem::Packet* rowPacket)
	{
		NetworkSystem::PK_I_DB_ANS_ID_PW* packet = (NetworkSystem::PK_I_DB_ANS_ID_PW*)rowPacket;
		SLogPrintAtFile(L"* id/pw result: %d", packet->result);

		NetworkSystem::Session* pClientSession = SESSIONMANAGER->GetSession(packet->ClientID);
		if (pClientSession == nullptr)
		{
			return;
		}

		NetworkSystem::PK_ANS_LOGIN ansPacket;
		ansPacket.IsSuccess = packet->result;
		pClientSession->SendPacket(&ansPacket);
	}
	));
}
