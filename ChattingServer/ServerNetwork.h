#pragma once

#define SERVERNETWORK ServerNetwork::GetInstance()

class ServerNetwork : Singleton<ServerNetwork>
{
private:
	HANDLE m_IOCP;
private:

	//�ʱ�ȭ
	void WSAInit();

public:
	ServerNetwork();
	~ServerNetwork();


};

