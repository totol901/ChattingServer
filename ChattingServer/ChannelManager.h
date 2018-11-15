/******************************************************************************
파일명	: ChannelManager.h
목적		: 채널 객체를 관리하는 매니저
******************************************************************************/
#pragma once

class ChannelManager : public Singleton<ChannelManager>
{
	friend ChannelManager;
private:
	map<string, Channel*>	m_mapStringChannels;
	map<UINT, Channel*>		m_mapIDChannels;

public:
	ChannelManager();
	~ChannelManager();

	bool		MakeChannelWithChannelName(string channelName);
	Channel*	FindChannelByName(const string& channelName);
	Channel*	FindChannelByID(UINT id);
	bool		DeleteChannelByName(const string& channelName);
	bool		DeleteChannelByID(const UINT& id);
	UINT		GetCanMakeMinimumChannelID();
	string		GetChannelList();
};
