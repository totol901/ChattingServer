#include "stdafx.h"
#include "ClientSessionManager.h"

namespace ServerEngine
{
	namespace NetworkSystem
	{
		ClientSessionManager::ClientSessionManager()
			:m_GenerateSessionID(0),
			m_bIsOn(true)
		{
			InitializeCriticalSection(&ManagerCS);
			m_HarbeatEndEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		}

		ClientSessionManager::~ClientSessionManager()
		{
			DeleteCriticalSection(&ManagerCS);
		}

		HRESULT ClientSessionManager::Init()
		{
			THREADPOOLMANAGER->MakeWork(
				bind(&ClientSessionManager::SendSessionHartBeats,this)
			);

			return S_OK;
		}

		void ClientSessionManager::Release()
		{
			m_bIsOn = false;
			//스래드가 종료될때까지 클래스 파괴 기다려줌
			WaitForSingleObject(m_HarbeatEndEvent, INFINITE);
			CloseHandle(m_HarbeatEndEvent);

			for (auto iter = m_setClientSession.begin();
				iter != m_setClientSession.end();
				iter++)
			{
				ClientSession* temp = (*iter);
				SAFE_DELETE(temp);
			}

			//for (auto iter = m_mapClientSession.begin();
			//	iter != m_mapClientSession.end();
			//	iter++)
			//{
			//	SAFE_DELETE(iter->second);
			//}
		}

		bool ClientSessionManager::AddClientSession(ClientSession * const clientSession)
		{
			EnterCriticalSection(&ManagerCS);
			if (m_setClientSession.find(clientSession) ==
				m_setClientSession.end())
			{
				m_setClientSession.insert((ClientSession*)clientSession);
				//m_mapClientSession.insert(
				//	make_pair((SOCKET)clientSession->GetSocket(),
				//	(ClientSession*)clientSession));
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

		bool ClientSessionManager::DeleteClientSession(ClientSession* cs)
		{
			if (m_setClientSession.empty())
			{
				return false;
			}
			EnterCriticalSection(&ManagerCS);
			auto iter = m_setClientSession.find(cs);

			if (iter == m_setClientSession.end())
			{
				return false;
			}

			//DeleteClientSessionID(iter->second->GetPlayerData()->GetPlayerID());

			m_setClientSession.erase(iter);
			LeaveCriticalSection(&ManagerCS);

			return true;
		}

		ClientSession* ClientSessionManager::FindClientSession(ClientSession* cs)
		{
			if (m_setClientSession.empty())
			{
				return nullptr;
			}
			EnterCriticalSection(&ManagerCS);
			auto iter = m_setClientSession.find(cs);
			LeaveCriticalSection(&ManagerCS);
			if (iter == m_setClientSession.end())
			{
				return nullptr;
			}

			return *iter;
		}

		void ClientSessionManager::SendSessionHartBeats()
		{
			while (m_bIsOn)
			{
				//10초 마다 스레드 하트비트 송신
				WaitForSingleObject(GetCurrentThread(), 1000);
				EnterCriticalSection(&ManagerCS);
				for (auto iter = m_setClientSession.begin();
					iter != m_setClientSession.end();
					iter++)
				{
					//iter->second->SendHeartBeat();
				}
				LeaveCriticalSection(&ManagerCS);
			}

			SetEvent(m_HarbeatEndEvent);
		}
	}
}
