/******************************************************************************
파일명	: LoginChattingClientSessionManager.h
목적		: 클라이언트 세션들을 관리함
******************************************************************************/
#pragma once

class LoginChattingClientSessionManager : 
	public Singleton<LoginChattingClientSessionManager>,
	public ClientSessionManager
{
	friend Singleton;
private:
	map<wstring, LoginChattingClientSession*> m_mapClientSessionID;	//로그인된 세션

private:
	LoginChattingClientSessionManager();
	~LoginChattingClientSessionManager();

public:
	HRESULT Init();
	void Release();
	
	bool			AddClientSessionID(wstring id , LoginChattingClientSession* const clientSession);
	
	bool			DeleteClientSessionID(wstring& id);
	
	LoginChattingClientSession*	FindClientSessionID(wstring id);

	
};
