#pragma once

class ChannelManager : public Singleton<ChannelManager>
{
	friend ChannelManager;
private:
	map<string, Channel*> m_mapStringChannels;
	map<UINT, Channel*> m_mapIDChannels;

	//vector<Channel> m_vecChannelList;

public:
	ChannelManager();
	~ChannelManager();

	bool MakeChannel(string channelName);
	Channel* FindChannelByName(const string& channelName);
	Channel* FindChannelByID(UINT id);
	bool DeleteChannelByName(const string& channelName);
	bool DeleteChannelByID(const UINT& id);
	string GetChannelList();
};
