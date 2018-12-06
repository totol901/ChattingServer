/******************************************************************************
파일명	: ChannelManager.h
목적		: 채널 객체를 관리하는 매니저
******************************************************************************/
#pragma once

class ChannelManager : public Singleton<ChannelManager>
{
	friend ChannelManager;
private:
	map<wstring, Channel*>	m_mapStringChannels;
	map<UINT, Channel*>		m_mapIDChannels;

public:
	ChannelManager();
	~ChannelManager();

	bool		ClientJoinTheChannel(ClientSession* clientSession, wstring channelName);
	bool		MakeChannelWithChannelName(wstring channelName);
	Channel*	FindChannelByName(const wstring& channelName);
	Channel*	FindChannelByID(UINT id);
	bool		DeleteChannelByName(const wstring& channelName);
	bool		DeleteChannelByID(const UINT& id);
	UINT		GetCanMakeMinimumChannelID();
	wstring		GetChannelList();
};
