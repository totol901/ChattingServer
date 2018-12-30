/******************************************************************************
���ϸ�	: ServerNetwork.h
����		: ������Ʈ��ũ�� �����
******************************************************************************/
#pragma once

class LoginChattingServerNetwork : 
	public ServerNetwork, 
	public Singleton<LoginChattingServerNetwork>
{
	friend Singleton;
private:
	LoginChattingServerNetwork();
	~LoginChattingServerNetwork();

	void CreateIOCPThreads();
	void CreateAcceptThread();

public:
	HRESULT Init();
	void Release();

	static bool IsDeleteSession(LoginChattingClientSession* pClientSession, DWORD bytesTransferred);
	static bool IsDeleteSessionCommon(LoginChattingClientSession* pClientSession, DWORD bytesTransferred);

	static unsigned int WINAPI AcceptRoop(LPVOID sNetwork);
	static unsigned int WINAPI CompletionClientSessionThread(LPVOID pComPort);

	static bool DisconnectLoginClientSession(LoginChattingClientSession* pClientSession);
};