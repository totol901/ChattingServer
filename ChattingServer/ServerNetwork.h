/******************************************************************************
파일명	: ServerNetwork.h
목적		: 서버네트워크를 담당함
******************************************************************************/
#pragma once

class ServerNetwork : public Singleton<ServerNetwork>
{
	friend Singleton;
private:
	HANDLE	m_IOCP;
	SOCKET	m_ListenSock;
	bool	m_Shutdown;

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
	const bool&		IsShutdown()	const { return m_Shutdown; }
	const SOCKET &	GetListenSock() const { return m_ListenSock; }
	const HANDLE &	GetIOCPHandle() const { return m_IOCP; }
};
