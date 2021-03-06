/******************************************************************************
파일명	: Channel.h
목적		: 채널 객체
******************************************************************************/
#pragma once

class Channel : public Object
{
private:
	UINT				m_ChannelID;
	set<ClientSession*> m_setClientSessions;
	string				m_name;

	bool IsChannelEmpty() { return m_setClientSessions.empty(); }

public:
	Channel(const string& channelName);
	~Channel();

	
	bool ChannelErase();

	bool InsertClientSession(ClientSession* clientsession);
	bool IsClientSessionIntoSet(ClientSession* clientsession);
	bool DeleteClientSession(ClientSession* clientsession);

	void SendPacketToChannelMember(T_PACKET& packet);

	string& GetName()		{ return m_name; }
	UINT	GetChannelID() const	{ return m_ChannelID; }
};
