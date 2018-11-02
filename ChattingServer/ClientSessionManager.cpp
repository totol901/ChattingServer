#include "stdafx.h"
#include "ClientSessionManager.h"

ClientSessionManager::ClientSessionManager()
	:m_GenerateSessionID(0)
{
}

ClientSessionManager::~ClientSessionManager()
{
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

	m_mapClientSession.erase(iter);
	return true;
}
