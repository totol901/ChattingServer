/******************************************************************************
���ϸ�	: ClientNetWork.h
����		: Ŭ���̾�Ʈ ��Ʈ��ũ ó��
******************************************************************************/
#pragma once

#define CLIENTNETWORK ClientNetwork::GetInstance()

class IOCPServerSession;
class ClientNetwork : public Singleton<ClientNetwork>
{
	friend Singleton;
private:
	HANDLE				m_IOCP;
	IOCPServerSession*	p_mServerSession;
	bool*				isOn;

private:
	ClientNetwork();
	~ClientNetwork();
	//�ʱ�ȭ
	void WSAInit();
	//������ ����
	void ConnectServer(const char* serverIp, const u_short& serverPort);

public:
	bool IsOn() { return *isOn; }
	//ison ���� �޸𸮸�ũ �Լ�
	void SetLinkIsOn(bool* ison);

	/****************************************************************************
	�Լ���	: Init
	����		: IP, Port�� ������ ������
	*****************************************************************************/
	void Init(char* serverIp, u_short serverPort);

	//IOCP�� ����� ���� ó�� ������
	static unsigned int WINAPI WorkThread(LPVOID param);

	//Ŭ���̾�Ʈ�� ����ϴ� �������� Get
	IOCPServerSession* GetServerSession() { return p_mServerSession; }
};
