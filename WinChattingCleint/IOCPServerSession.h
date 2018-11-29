/******************************************************************************
파일명	: IOCPServerSession.h
목적		: 서버 세션 객체
******************************************************************************/
#pragma once

class ServerSessionParser;
class IOCPServerSession : public Session
{
private:
	bool* m_IsOn;
	ServerSessionParser* m_pServerSessionParser;

	//버퍼 송신
	void Send(WSABUF wsaBuf);
	//버퍼 받기
	void Recv(WSABUF wsabuf);
	//버퍼 받는중인지 판별
	bool IsRecving(size_t transferSize);

public:
	IOCPServerSession();
	~IOCPServerSession();

	ServerSessionParser* GetServerSessionParser() { return m_pServerSessionParser; }
	
	//ison 변수 메모리링크 함수
	void SetLinkIsOn(bool* ison);

	/****************************************************************************
	함수명	: PacketParsing
	설명		: 들어온 페킷을 size,type, buff로 파싱함
	*****************************************************************************/
	bool PacketParsing(T_PACKET* pakcet);

	/****************************************************************************
	함수명	: OnConnect
	설명		: 서버에 접속
	*****************************************************************************/
	void OnConnect(const char* serverIp, u_short serverPort);

	/****************************************************************************
	함수명	: sendPacket
	설명		: packet데이터를 ioData_ 형태로 변형 후 send함
	*****************************************************************************/
	void SendPacket(T_PACKET packet);

	/****************************************************************************
	함수명	: Disconnect
	설명		: 접속 종료
	*****************************************************************************/
	void Disconnect();

	/****************************************************************************
	함수명	: onRecv
	설명		: 데이터 분석, Recv 상태인 패킷 리턴해줌
	*****************************************************************************/
	T_PACKET* OnRecv(size_t transferSize);
	
	/****************************************************************************
	함수명	: onSend
	설명		: 페킷 보내기 On이면 패킷 송신함
	*****************************************************************************/
	void OnSend(size_t transferSize);

	/****************************************************************************
	함수명	: RecvStandBy
	설명		: ioData[IO_READ] 버퍼 초기화, IOCP의 recv 비동기 실행
	*****************************************************************************/
	void RecvStandBy();
};

