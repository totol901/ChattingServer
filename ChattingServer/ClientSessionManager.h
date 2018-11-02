#pragma once

class ClientSessionManager : public Singleton<ClientSessionManager>
{
	friend Singleton;
private:
	UINT						m_GenerateSessionID;
	map<SOCKET, ClientSession*> m_mapClientSession;

private:
	ClientSessionManager();
	~ClientSessionManager();

	UINT GetGenerateSessionID();

public:
	bool			AddClientSession( ClientSession* const clientSession);
	//ClientSession*	FindSession(const SOCKET& socket);
	bool			DeleteClientSession(const SOCKET& socket);

	const size_t GetCCU() const { return m_mapClientSession.size(); }
};
