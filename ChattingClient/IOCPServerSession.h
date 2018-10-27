#pragma once

class IOCPServerSession : public Session
{
private:
	IOData m_arrIOData[2];
	bool m_bConnected;
	
public:
	IOCPServerSession();
	~IOCPServerSession();

	void sendPacket(char *packet);

	void Disconnect();

	void OnConnect(const char* serverIp, u_short serverPort);

	void Recv(WSABUF wsabuf);
	string* OnRecv(size_t transferSize);
	bool IsRecving(size_t transferSize);

	void Send(WSABUF wsaBuf);
	/****************************************************************************
	함수명	: onSend
	설명		: 페킷 보내기 On이면 패킷 송신함
	리턴값	: 없음
	매개변수	: size_t
	*****************************************************************************/
	void			OnSend(size_t transferSize);

	/****************************************************************************
	함수명	: sendPacket
	설명		: packet데이터를 ioData_ 형태로 변형 후 send함
	리턴값	: 없음
	매개변수	: Packet*
	*****************************************************************************/
	void		    sendPacket();

	/****************************************************************************
	함수명	: onRecv
	설명		: 데이터 분석, Recv 상태인 패킷 리턴해줌
	리턴값	: Package*
	매개변수	: size_t
	*****************************************************************************/
	//Package*		onRecv(size_t transferSize);

	/****************************************************************************
	함수명	: RecvStandBy
	설명		: ioData[IO_READ] 버퍼 초기화, IOCP의 recv 비동기 실행
	리턴값	: 없음
	매개변수	: 없음
	*****************************************************************************/
	void			RecvStandBy();

};

