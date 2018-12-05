#include "stdafx.h"
#include "ClientSessionManager.h"

static CRITICAL_SECTION ManagerCS;

ClientSessionManager::ClientSessionManager()
	:m_GenerateSessionID(0)
{
	InitializeCriticalSection(&ManagerCS);
}

ClientSessionManager::~ClientSessionManager()
{
	DeleteCriticalSection(&ManagerCS);
	for (auto iter = m_mapClientSession.begin();
		iter != m_mapClientSession.end();
		iter++)
	{
		SAFE_DELETE(iter->second);
	}
}

bool ClientSessionManager::AddClientSession( ClientSession * const clientSession)
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

bool ClientSessionManager::AddClientSessionID(string id, ClientSession * const clientSession)
{
	EnterCriticalSection(&ManagerCS);
	if (m_mapClientSessionID.find(clientSession->GetPlayerData()->GetPlayerID()) ==
		m_mapClientSessionID.end())
	{
		
		m_mapClientSessionID.insert(
			make_pair(clientSession->GetPlayerData()->GetPlayerID(),
			(ClientSession*)clientSession));
		LeaveCriticalSection(&ManagerCS);
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

	if (iter ==	m_mapClientSession.end())
	{
		return false;
	}

	//DeleteClientSessionID(iter->second->GetPlayerData()->GetPlayerID());
	
	m_mapClientSession.erase(iter);
	LeaveCriticalSection(&ManagerCS);

	return true;
}

bool ClientSessionManager::DeleteClientSessionID(string& id)
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

ClientSession* ClientSessionManager::FindClientSession(SOCKET socket)
{
	if (m_mapClientSessionID.empty())
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

ClientSession * ClientSessionManager::FindClientSessionID(string id)
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
