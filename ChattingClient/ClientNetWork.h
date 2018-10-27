/******************************************************************************
���ϸ�	: ClientNetWork.h
����		: Ŭ���̾�Ʈ ��Ʈ��ũ ó��
******************************************************************************/
#pragma once

class IOCPServerSession;
class ClientNetwork
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
	ClientNetwork(char* serverIp, u_short serverPort);
	~ClientNetwork();

	IOCPServerSession* GetServerSession() { return p_mServerSession; }

	static unsigned int WINAPI WorkThread(LPVOID param);
};
