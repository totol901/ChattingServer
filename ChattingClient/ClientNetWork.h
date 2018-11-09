/******************************************************************************
파일명	: ClientNetWork.h
목적		: 클라이언트 네트워크 처리
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
	//초기화
	void WSAInit();
	//서버에 접속
	void ConnectServer(const char* serverIp, const u_short& serverPort);

public:
	bool IsOn() { return *isOn; }
	//ison 변수 메모리링크 함수
	void SetLinkIsOn(bool* ison);

	/****************************************************************************
	함수명	: Init
	설명		: IP, Port의 서버에 접속함
	*****************************************************************************/
	void Init(char* serverIp, u_short serverPort);

	//IOCP를 사용한 소켓 처리 스레드
	static unsigned int WINAPI WorkThread(LPVOID param);

	//클라이언트와 통신하는 서버세션 Get
	IOCPServerSession* GetServerSession() { return p_mServerSession; }
};
