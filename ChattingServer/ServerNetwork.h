#pragma once

class ServerNetwork : public Singleton<ServerNetwork>
{
	friend Singleton;
private:
	HANDLE m_IOCP;
	SOCKET m_ListenSock;

private:
	ServerNetwork();
	~ServerNetwork();

	void CreateListen();

public:
	void Init();

	void CreateIOCP();
	
	void AcceptRoop();

	static unsigned int WINAPI CompletionClientSessionThread(LPVOID pComPort);
};
