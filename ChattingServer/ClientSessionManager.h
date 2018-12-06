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
	map<wstring, ClientSession*> m_mapClientSessionID;	//로그인된 세션

private:
	ClientSessionManager();
	~ClientSessionManager();

	const UINT GetGenerateSessionID();

public:
	bool			AddClientSession( ClientSession* const clientSession);
	bool			AddClientSessionID(wstring id ,ClientSession* const clientSession);
	bool			DeleteClientSession(const SOCKET& socket);
	bool			DeleteClientSessionID(wstring& id);
	ClientSession*	FindClientSession(SOCKET socket);
	ClientSession*	FindClientSessionID(wstring id);

	const size_t GetCCU() const { return m_mapClientSession.size(); }
};
