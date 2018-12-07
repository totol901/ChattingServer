/******************************************************************************
���ϸ�	: ServerNetwork.h
����		: ������Ʈ��ũ�� �����
******************************************************************************/
#pragma once

class ServerNetwork : public Singleton<ServerNetwork>
{
	friend Singleton;
private:
	vector<HANDLE> m_vecIOCPThread;
	HANDLE	m_IOCP;
	SOCKET	m_ListenSock;
	bool	m_Shutdown;

private:
	ServerNetwork();
	~ServerNetwork();

	void CreateListen();
	void CreateIOCP();
	void CreateIOCPThreads();
	void CreateAcceptThread();

public:
	HRESULT Init();
	void Release();
	
	static unsigned int WINAPI AcceptRoop(LPVOID sNetwork);
	static unsigned int WINAPI CompletionClientSessionThread(LPVOID pComPort);

	static bool DisconnectClientSession(SOCKET socket);
	static bool DisconnectLoginClientSession(ClientSession* pClientSession);
public:
	const bool&		IsShutdown()	const { return m_Shutdown; }
	const SOCKET &	GetListenSock() const { return m_ListenSock; }
	const HANDLE &	GetIOCPHandle() const { return m_IOCP; }
};
