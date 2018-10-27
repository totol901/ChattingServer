/******************************************************************************
파일명	: ClientNetWork.h
목적		: 클라이언트 네트워크 처리
******************************************************************************/
#pragma once

class IOCPServerSession;
class ClientNetwork
{
private:
	HANDLE m_IOCP;
	IOCPServerSession* p_mServerSession;

private:
	//초기화
	void WSAInit();
	//서버에 접속
	void ConnectServer(const char* serverIp, const u_short& serverPort);

public:
	ClientNetwork(char* serverIp, u_short serverPort);
	~ClientNetwork();

	IOCPServerSession* GetServerSession() { return p_mServerSession; }

	static unsigned int WINAPI WorkThread(LPVOID param);
};
