#pragma once

class ClientSessionManager : public Singleton<ClientSessionManager>
{
	friend Singleton;
private:
	UINT m_GenerateSessionID;
	map<SOCKET, ClientSession*> m_mapClientSession;

private:
	ClientSessionManager();
	~ClientSessionManager();

	UINT GetGenerateSessionID();

public:
	bool AddClientSession( ClientSession* const clientSession);
	
	ClientSession* FindSession(SOCKET socket);
	bool DeleteClientSession(const SOCKET& socket);
};
