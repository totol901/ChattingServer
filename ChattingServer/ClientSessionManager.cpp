#include "stdafx.h"
#include "ClientSessionManager.h"


ClientSessionManager::ClientSessionManager()
	:m_GenerateSessionID(0)
{
}


ClientSessionManager::~ClientSessionManager()
{
}

UINT ClientSessionManager::GetGenerateSessionID()
{
	return m_GenerateSessionID++;
}
