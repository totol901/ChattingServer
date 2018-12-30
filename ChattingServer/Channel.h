/******************************************************************************
���ϸ�	: Channel.h
����		: ä�� ��ü
******************************************************************************/
#pragma once

class Channel : public Object
{
private:
	UINT				m_ChannelID;
	unordered_set<ClientSession*> m_setClientSessions;
	wstring				m_name;

	bool IsChannelEmpty() { return m_setClientSessions.empty(); }

public:
	Channel(const wstring& channelName);
	~Channel();

	bool ChannelErase();

	bool InsertClientSession(ClientSession* clientsession);
	bool IsClientSessionIntoSet(ClientSession* clientsession);
	bool DeleteClientSession(ClientSession* clientsession);

	void SendPacketToChannelMember(NetworkSystem::Stream& packetData);
	void SendPacketChannelData(ClientSession* clientsession);

	wstring& GetName()		{ return m_name; }
	UINT	GetChannelID() const	{ return m_ChannelID; }
};
