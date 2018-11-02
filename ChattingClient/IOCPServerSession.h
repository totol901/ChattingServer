#pragma once

class IOCPServerSession : public Session
{
private:
	bool m_bConnected;

	void Send(WSABUF wsaBuf);
	void Recv(WSABUF wsabuf);
	bool IsRecving(size_t transferSize);

public:
	IOCPServerSession();
	~IOCPServerSession();

	bool PacketParsing(T_PACKET* pakcet);

	/****************************************************************************
	함수명	: sendPacket
	설명		: packet데이터를 ioData_ 형태로 변형 후 send함
	리턴값	: 없음
	매개변수	: Packet*
	*****************************************************************************/
	void SendPacket(T_PACKET packet);

	void Disconnect();

	void OnConnect(const char* serverIp, u_short serverPort);
	
	/****************************************************************************
	함수명	: onRecv
	설명		: 데이터 분석, Recv 상태인 패킷 리턴해줌
	리턴값	: Package*
	매개변수	: size_t
	*****************************************************************************/
	T_PACKET* OnRecv(size_t transferSize);
	
	/****************************************************************************
	함수명	: onSend
	설명		: 페킷 보내기 On이면 패킷 송신함
	리턴값	: 없음
	매개변수	: size_t
	*****************************************************************************/
	void OnSend(size_t transferSize);

	/****************************************************************************
	함수명	: RecvStandBy
	설명		: ioData[IO_READ] 버퍼 초기화, IOCP의 recv 비동기 실행
	리턴값	: 없음
	매개변수	: 없음
	*****************************************************************************/
	void RecvStandBy();
};

