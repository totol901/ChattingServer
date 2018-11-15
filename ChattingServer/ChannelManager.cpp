#include "stdafx.h"
#include "ChannelManager.h"

ChannelManager::ChannelManager()
{
}

ChannelManager::~ChannelManager()
{
}

bool ChannelManager::MakeChannelWithChannelName(string channelName)
{
	auto iter = m_mapStringChannels.find(channelName);
	if (iter == m_mapStringChannels.end())
	{
		Channel* channel = new Channel(channelName);
		m_mapStringChannels.insert(make_pair(channelName, channel));
		m_mapIDChannels.insert(make_pair(channel->GetChannelID(), channel));
		
		return true;
	}

	return false;
}

Channel * ChannelManager::FindChannelByName(const string& channelName)
{
	auto iter = m_mapStringChannels.find(channelName);
	if (iter == m_mapStringChannels.end())
	{
		return nullptr;
	}

	return iter->second;
}

Channel * ChannelManager::FindChannelByID(UINT id)
{
	auto iter = m_mapIDChannels.find(id);
	if (iter == m_mapIDChannels.end())
	{
		return nullptr;
	}

	return iter->second;
}

bool ChannelManager::DeleteChannelByName(const string & channelName)
{
	auto iterString = m_mapStringChannels.find(channelName);
	auto iterID = m_mapIDChannels.find(iterString->second->GetChannelID());

	if (iterString == m_mapStringChannels.end())
	{
		SLogPrintAtFile("! 채널 매니저의 채널 삭제 불가능");
	}
	else
	{
		m_mapStringChannels.erase(iterString);
		return false;
	}

	if (iterID == m_mapIDChannels.end())
	{
		SLogPrintAtFile("! 채널 매니저의 채널 삭제 불가능");
		return false;
	}
	else
	{
		m_mapIDChannels.erase(iterID);
	}

	return true;
}

bool ChannelManager::DeleteChannelByID(const UINT & id)
{
	auto iterID = m_mapIDChannels.find(id);
	auto iterString = m_mapStringChannels.find(iterID->second->GetName());

	if (iterString == m_mapStringChannels.end())
	{
		SLogPrintAtFile("! 채널 매니저의 채널 삭제 불가능");
		return false;
	}
	else
	{
		m_mapStringChannels.erase(iterString);
	}

	if (iterID == m_mapIDChannels.end())
	{
		SLogPrintAtFile("! 채널 매니저의 채널 삭제 불가능");
		return false;
	}
	else
	{
		m_mapIDChannels.erase(iterID);
	}

	return true;
}

UINT ChannelManager::GetCanMakeMinimumChannelID()
{
	UINT ID = 0;
	while (true)
	{
		auto iter = m_mapIDChannels.find(ID);
		if (iter == m_mapIDChannels.end())
		{
			return ID;
		}

		ID++;
		ASSERT(ID < UINT_MAX);
	}

	return UINT_MAX;
}

string ChannelManager::GetChannelList()
{
	string temp;
	for (auto iter = m_mapIDChannels.begin();
		iter != m_mapIDChannels.end(); iter++)
	{
		char num[8];
		_itoa_s((int)iter->first, num, 10);
		temp += num;
		temp += ". ";
		temp += iter->second->GetName();
		temp += "\n";
	}

	return temp;
}
