#include "stdafx.h"
#include "ChannelManager.h"

ChannelManager::ChannelManager()
{
}

ChannelManager::~ChannelManager()
{
}

bool ChannelManager::ClientJoinTheChannel(LoginChattingClientSession* clientSession, wstring channelName)
{
	Channel* channel = CHANNELMANAGER->FindChannelByName(channelName);
	if (channel)
	{
		//채널에 입장시킴
		channel->InsertClientSession(clientSession);

		//플레이어 데이터에 입장한 채널 세팅해줌
		clientSession->GetPlayerData()->SetChannel(channel);
		clientSession->GetPlayerData()->SetPlayerState(PLAYER_IN_CHANNEL);
		return true;
	}

	return false;
}

bool ChannelManager::MakeChannelWithChannelName(wstring channelName)
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

Channel * ChannelManager::FindChannelByName(const wstring& channelName)
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
	if (m_mapIDChannels.size() == 0)
	{
		return nullptr;
	}
	auto iter = m_mapIDChannels.find(id);
	if (iter == m_mapIDChannels.end())
	{
		return nullptr;
	}

	return iter->second;
}

bool ChannelManager::DeleteChannelByName(const wstring & channelName)
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

wstring ChannelManager::GetChannelList()
{
	wstring temp;
	for (auto iter = m_mapIDChannels.begin();
		iter != m_mapIDChannels.end(); iter++)
	{
		TCHAR num[8];
		_itow_s((int)iter->first, num, 10);
		temp += num;
		temp += L". ";
		temp += iter->second->GetName();
		temp += L"\n";
	}

	return temp;
}
