/******************************************************************************
���ϸ�	: Channel.h
����		: ä�� ��ü
******************************************************************************/
#pragma once

class Channel : public Object
{
private:
	set<ClientSession*> m_setClientSessions;
	string				m_name;

public:
	Channel(const string& channelName);
	~Channel();

	bool IsChannelEmpty() { return m_setClientSessions.empty(); }

	bool InsertClientSession(ClientSession* clientsession);
	bool IsClientSessionIntoSet(ClientSession* clientsession);
	bool DeleteClientSession(ClientSession* clientsession);

	void SendPacketToChannelMember(T_PACKET& packet);

	string& GetName() { return m_name; }
};
