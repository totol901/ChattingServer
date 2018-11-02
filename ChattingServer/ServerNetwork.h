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
	
	static unsigned int WINAPI AcceptRoop(LPVOID sNetwork);

	static unsigned int WINAPI CompletionClientSessionThread(LPVOID pComPort);

public:
	const SOCKET & GetListenSock() const { return m_ListenSock; }
	const HANDLE & GetIOCPHandle() const { return m_IOCP; }
};
