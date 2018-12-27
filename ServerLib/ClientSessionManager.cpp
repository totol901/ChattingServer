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

		void ClientSessionManager::SendSessionHartBeats()
		{
			while (m_bIsOn)
			{
				//10초 마다 스레드 하트비트 송신
				WaitForSingleObject(GetCurrentThread(), 1000);
				EnterCriticalSection(&ManagerCS);
				for (auto iter = m_mapClientSession.begin();
					iter != m_mapClientSession.end();
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
