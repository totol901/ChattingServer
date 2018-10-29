/******************************************************************************
���ϸ�	: ClientNetWork.h
����		: Ŭ���̾�Ʈ ��Ʈ��ũ ó��
******************************************************************************/
#pragma once

#define CLIENTNETWORK ClientNetwork::GetInstance()

class IOCPServerSession;
class ClientNetwork : public Singleton<ClientNetwork>
{
private:
	HANDLE m_IOCP;
	IOCPServerSession* p_mServerSession;

private:
	
	//�ʱ�ȭ
	void WSAInit();
	//������ ����
	void ConnectServer(const char* serverIp, const u_short& serverPort);

public:
	ClientNetwork();
	~ClientNetwork();

	void Init(char* serverIp, u_short serverPort);

	IOCPServerSession* GetServerSession() { return p_mServerSession; }

	static unsigned int WINAPI WorkThread(LPVOID param);
};
