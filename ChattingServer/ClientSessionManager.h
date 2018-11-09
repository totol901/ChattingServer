/******************************************************************************
파일명	: ClientSessionManager.h
목적		: 클라이언트 세션들을 관리함
******************************************************************************/
#pragma once

class ClientSessionManager : public Singleton<ClientSessionManager>
{
	friend Singleton;
private:
	UINT						m_GenerateSessionID;
	map<SOCKET, ClientSession*> m_mapClientSession;		//억셉트된 세션
	map<string, ClientSession*> m_mapClientSessionID;	//로그인된 세션

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
