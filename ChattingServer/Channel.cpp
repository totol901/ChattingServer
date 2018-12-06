#include "stdafx.h"
#include "Channel.h"

Channel::Channel(const wstring& channelName)
	:Object()
{
	m_ChannelID = CHANNELMANAGER->GetCanMakeMinimumChannelID();
	m_type = OBJECT_CHANNEL;
	m_name = channelName;
}

Channel::~Channel()
{
}

bool Channel::ChannelErase()
{
	if (!IsChannelEmpty())
	{
		return false;
	}

	CHANNELMANAGER->DeleteChannelByID(this->GetChannelID());

	return true;
}

bool Channel::InsertClientSession(ClientSession * clientsession)
{
	auto iter = m_setClientSessions.find(clientsession);
	if (iter == m_setClientSessions.end())
	{
		m_setClientSessions.insert(clientsession);
		return true;
	}
	
	return false;
}

bool Channel::IsClientSessionIntoSet(ClientSession * clientsession)
{
	auto iter = m_setClientSessions.find(clientsession);
	if (iter == m_setClientSessions.end())
	{
		return false;
	}

	return true;
}

bool Channel::DeleteClientSession(ClientSession * clientsession)
{
	auto iter = m_setClientSessions.find(clientsession);
	if (iter == m_setClientSessions.end())
	{
		return false;
	}

	m_setClientSessions.erase(iter);
	return true;
}

void Channel::SendPacketToChannelMember(T_PACKET& packet)
{
	for (auto iter = m_setClientSessions.begin();
		iter != m_setClientSessions.end();
		iter++)
	{
		(*iter)->SendPacket(packet);
	}
}
