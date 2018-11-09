/******************************************************************************
파일명	: IOData.h
목적		: IOCP의 오버렙드에 들어갈 객체
******************************************************************************/
#pragma once

enum IO_OPERATION
{
	IO_READ,
	IO_WRITE,
	IO_ERROR,
};

class IOData
{
private:
	WSAOVERLAPPED	m_Overlapped;
	IO_OPERATION	m_IOType;
	size_t			m_TotalBytes;
	size_t			m_CurrentBytes;
	T_PACKET		m_Packet;

public:
	IOData();
	~IOData();

	/****************************************************************************
	함수명	: clear
	설명		: IoData 클래스의 데이터를 0으로 초기화함
	*****************************************************************************/
	void Clear();

	/****************************************************************************
	함수명	: needMoreIO
	설명		: IO가 더 필요한지 판별함
	*****************************************************************************/
	bool NeedMoreIO(size_t transferSize);

	/****************************************************************************
	함수명	: setupTotalBytes
	설명		: totalBytes_를 설정함, 설정 되있을시 무시 후 패킷 크기 반환
	*****************************************************************************/
	int32_t SetTotalBytes();

	/****************************************************************************
	함수명	: wsabuf
	설명		: 현재까지 온 버퍼, 길이 버퍼를 리턴함
	*****************************************************************************/
	WSABUF GetCurrentWSABuf();

	/****************************************************************************
	함수명	: setData
	설명		: buffer_에 stream의 크기를 헤더로, 버퍼를 뒤에 추가해주는 패킷 만듬, totalBytes_ 갱신
	*****************************************************************************/
	bool SetData(T_PACKET packet);

public:
	//get함수
	const size_t& GetTotalByte() const { return m_TotalBytes; }
	const IO_OPERATION& GetType() const { return m_IOType; }
	T_PACKET * const GetptPacket() { return &m_Packet; }
	LPWSAOVERLAPPED const GetptOverlapped() { return &m_Overlapped; }

	//set함수
	void SetType(const IO_OPERATION& type) { m_IOType = type; }
};
