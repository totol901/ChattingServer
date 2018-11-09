#include "stdafx.h"
#include "ClientSessionManager.h"

ClientSessionManager::ClientSessionManager()
	:m_GenerateSessionID(0)
{
}

ClientSessionManager::~ClientSessionManager()
{
	for (auto iter = m_mapClientSession.begin();
		iter != m_mapClientSession.end();
		iter++)
	{
		SAFE_DELETE(iter->second);
	}
}

bool ClientSessionManager::AddClientSession( ClientSession * const clientSession)
{
	if (m_mapClientSession.find(clientSession->GetSocket()) ==
		m_mapClientSession.end())
	{
		m_mapClientSession.insert(
			make_pair((SOCKET)clientSession->GetSocket(),
			(ClientSession*)clientSession));
		clientSession->SetId(GetGenerateSessionID());
		return true;
	}
	
	return false;
}

bool ClientSessionManager::AddClientSessionID(string id, ClientSession * const clientSession)
{
	if (m_mapClientSessionID.find(clientSession->GetPlayerData()->GetPlayerID()) ==
		m_mapClientSessionID.end())
	{
		m_mapClientSessionID.insert(
			make_pair(clientSession->GetPlayerData()->GetPlayerID(),
			(ClientSession*)clientSession));
		return true;
	}

	return false;
}

UINT ClientSessionManager::GetGenerateSessionID()
{
	return m_GenerateSessionID++;
}

bool ClientSessionManager::DeleteClientSession(const SOCKET & socket)
{
	auto iter = m_mapClientSession.find(socket);

	if (iter ==	m_mapClientSession.end())
	{
		return false;
	}

	//DeleteClientSessionID(iter->second->GetPlayerData()->GetPlayerID());

	m_mapClientSession.erase(iter);

	return true;
}

bool ClientSessionManager::DeleteClientSessionID(string& id)
{
	auto iter = m_mapClientSessionID.find(id);
	if (iter == m_mapClientSessionID.end())
	{
		return false;
	}

	m_mapClientSessionID.erase(iter);
	return true;
}

ClientSession* ClientSessionManager::FindClientSession(SOCKET socket)
{
	auto iter = m_mapClientSession.find(socket);
	if (iter == m_mapClientSession.end())
	{
		return nullptr;
	}

	return iter->second;
}

ClientSession * ClientSessionManager::FindClientSessionID(string id)
{
	auto iter = m_mapClientSessionID.find(id);
	if (iter == m_mapClientSessionID.end())
	{
		return nullptr;
	}

	return iter->second;
}
