#pragma once

class ClientSessionParser;
class ClientSession : public Session
{
private:
	bool m_bConnected;
	void Send(const WSABUF& wsaBuf);
	void Recv(const WSABUF& wsabuf);
	bool IsRecving(const size_t& transferSize);

	void ReqLogin(T_PACKET* packet);
	void ReqCreateID(T_PACKET* packet);

	ClientSessionParser* m_SessionParser;

public:
	ClientSession();
	~ClientSession();

	IOData* const GetptIOData(const int& type); const

	bool PacketParsing(T_PACKET* const pakcet);

	/****************************************************************************
	함수명	: sendPacket
	설명		: packet데이터를 ioData_ 형태로 변형 후 send함
	리턴값	: 없음
	매개변수	: Packet*
	*****************************************************************************/
	void SendPacket(const T_PACKET& packet);

	void Disconnect();

	/****************************************************************************
	함수명	: onRecv
	설명		: 데이터 분석, Recv 상태인 패킷 리턴해줌
	리턴값	: Package*
	매개변수	: size_t
	*****************************************************************************/
	T_PACKET* OnRecv(const size_t& transferSize);

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
