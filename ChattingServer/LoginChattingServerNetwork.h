/******************************************************************************
파일명	: ServerNetwork.h
목적		: 서버네트워크를 담당함
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
	
	static unsigned int WINAPI AcceptRoop(LPVOID sNetwork);
	static unsigned int WINAPI CompletionClientSessionThread(LPVOID pComPort);

	static bool DisconnectClientSession(SOCKET socket);
	static bool DisconnectLoginClientSession(LoginChattingClientSession* pClientSession);

};
