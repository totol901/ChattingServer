/******************************************************************************
���ϸ�	: LoginChattingClientSessionManager.h
����		: Ŭ���̾�Ʈ ���ǵ��� ������
******************************************************************************/
#pragma once

class LoginChattingClientSessionManager : 
	public Singleton<LoginChattingClientSessionManager>,
	public ClientSessionManager
{
	friend Singleton;
private:
	map<wstring, LoginChattingClientSession*> m_mapClientSessionID;	//�α��ε� ����

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
