#include "stdafx.h"
#include "LoginChattingClientSessionManager.h"

LoginChattingClientSessionManager::LoginChattingClientSessionManager()
	:ClientSessionManager()
{
}

LoginChattingClientSessionManager::~LoginChattingClientSessionManager()
{
}

HRESULT LoginChattingClientSessionManager::Init()
{
	HRESULT hr = ClientSessionManager::Init();

	return hr;
}

void LoginChattingClientSessionManager::Release()
{
	ClientSessionManager::Release();
}

bool LoginChattingClientSessionManager::AddClientSessionID(wstring id, LoginChattingClientSession * const clientSession)
{
	EnterCriticalSection(&ManagerCS);
	if (m_mapClientSessionID.find(clientSession->GetPlayerData()->GetPlayerID()) ==
		m_mapClientSessionID.end())
	{
		
		m_mapClientSessionID.insert(
			make_pair(clientSession->GetPlayerData()->GetPlayerID(),
			(LoginChattingClientSession*)clientSession));
		LeaveCriticalSection(&ManagerCS);
		return true;
	}

	return false;
}

bool LoginChattingClientSessionManager::DeleteClientSessionID(wstring& id)
{
	if (m_mapClientSessionID.empty())
	{
		return false;
	}
	EnterCriticalSection(&ManagerCS);
	auto iter = m_mapClientSessionID.find(id);
	if (iter == m_mapClientSessionID.end())
	{
		return false;
	}
	
	m_mapClientSessionID.erase(iter);
	LeaveCriticalSection(&ManagerCS);
	return true;
}

LoginChattingClientSession * LoginChattingClientSessionManager::FindClientSessionID(wstring id)
{
	if (m_mapClientSessionID.empty())
	{
		return nullptr;
	}
	EnterCriticalSection(&ManagerCS);
	auto iter = m_mapClientSessionID.find(id);
	LeaveCriticalSection(&ManagerCS);
	if (iter == m_mapClientSessionID.end())
	{
		return nullptr;
	}

	return iter->second;
}
