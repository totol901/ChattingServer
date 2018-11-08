#pragma once

class ClientSessionManager : public Singleton<ClientSessionManager>
{
	friend Singleton;
private:
	UINT						m_GenerateSessionID;
	map<SOCKET, ClientSession*> m_mapClientSession;//撅剂飘等 技记
	map<string, ClientSession*> m_mapClientSessionID;//肺弊牢等 技记

private:
	ClientSessionManager();
	~ClientSessionManager();

	UINT GetGenerateSessionID();

public:
	bool			AddClientSession( ClientSession* const clientSession);
	bool			AddClientSessionID(string id ,ClientSession* const clientSession);
	bool			DeleteClientSession(const SOCKET& socket);
	bool			DeleteClientSessionID(string& id);
	ClientSession*	FindClientSession(SOCKET socket);
	ClientSession*	FindClientSessionID(string id);

	const size_t GetCCU() const { return m_mapClientSession.size(); }
};
