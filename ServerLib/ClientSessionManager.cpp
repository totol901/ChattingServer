#include "stdafx.h"
#include "ClientSessionManager.h"

namespace ServerEngine
{
	namespace NetworkSystem
	{
		ClientSessionManager::ClientSessionManager()
			:m_GenerateSessionID(0)
		{
			InitializeCriticalSection(&ManagerCS);
		}

		ClientSessionManager::~ClientSessionManager()
		{
			DeleteCriticalSection(&ManagerCS);
		}

		HRESULT ClientSessionManager::Init()
		{
			return S_OK;
		}

		void ClientSessionManager::Release()
		{
			for (auto iter = m_mapClientSession.begin();
				iter != m_mapClientSession.end();
				iter++)
			{
				SAFE_DELETE(iter->second);
			}
		}

		bool ClientSessionManager::AddClientSession(ClientSession * const clientSession)
		{
			EnterCriticalSection(&ManagerCS);
			if (m_mapClientSession.find(clientSession->GetSocket()) ==
				m_mapClientSession.end())
			{

				m_mapClientSession.insert(
					make_pair((SOCKET)clientSession->GetSocket(),
					(ClientSession*)clientSession));
				LeaveCriticalSection(&ManagerCS);
				clientSession->SetId(GetGenerateSessionID());


				return true;
			}

			return false;
		}



		const UINT ClientSessionManager::GetGenerateSessionID()
		{
			return m_GenerateSessionID++;
		}

		bool ClientSessionManager::DeleteClientSession(const SOCKET & socket)
		{
			if (m_mapClientSession.empty())
			{
				return false;
			}
			EnterCriticalSection(&ManagerCS);
			auto iter = m_mapClientSession.find(socket);

			if (iter == m_mapClientSession.end())
			{
				return false;
			}

			//DeleteClientSessionID(iter->second->GetPlayerData()->GetPlayerID());

			m_mapClientSession.erase(iter);
			LeaveCriticalSection(&ManagerCS);

			return true;
		}



		ClientSession* ClientSessionManager::FindClientSession(SOCKET socket)
		{
			if (m_mapClientSession.empty())
			{
				return nullptr;
			}
			EnterCriticalSection(&ManagerCS);
			auto iter = m_mapClientSession.find(socket);
			LeaveCriticalSection(&ManagerCS);
			if (iter == m_mapClientSession.end())
			{
				return nullptr;
			}

			return iter->second;
		}

	}
}
