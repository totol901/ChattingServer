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
	WSAOVERLAPPED m_Overlapped;
	IO_OPERATION m_IOType;
	size_t m_TotalBytes;
	size_t m_CurrentBytes;
	char m_Buff[SOCKET_BUFF_SIZE*2];

public:
	IOData();
	~IOData();

	/****************************************************************************
	함수명	: clear
	설명		: IoData 클래스의 데이터를 0으로 초기화함
	리턴값	: 없음
	매개변수	: 없음
	*****************************************************************************/
	void Clear();

	/****************************************************************************
	함수명	: needMoreIO
	설명		: IO가 더 필요한지 판별함
	리턴값	: bool
	매개변수	: size_t
	*****************************************************************************/
	bool NeedMoreIO(size_t transferSize);

	/****************************************************************************
	함수명	: setupTotalBytes
	설명		: totalBytes_를 설정함, 설정 되있을시 무시 후 패킷 크기 반환
	리턴값	: int32_t
	매개변수	: 없음
	*****************************************************************************/
	int32_t SetTotalBytes();

	/****************************************************************************
	함수명	: wsabuf
	설명		: 현재까지 온 버퍼, 길이 버퍼를 리턴함
	리턴값	: WSABUF
	매개변수	: 없음
	*****************************************************************************/
	WSABUF GetCurrentWSABuf();

	/****************************************************************************
	함수명	: setData
	설명		: buffer_에 stream의 크기를 헤더로, 버퍼를 뒤에 추가해주는 패킷 만듬, totalBytes_ 갱신
	리턴값	: bool
	매개변수	: char*
	*****************************************************************************/
	bool SetData(T_PACKET stream);

public:
	//get함수
	size_t GetTotalByte() { return m_TotalBytes; }
	IO_OPERATION GetType() { return m_IOType; }
	char * GetDataBuff() { return m_Buff; }
	LPWSAOVERLAPPED GetOverlapped() { return &m_Overlapped; }

	//set함수
	void SetType(IO_OPERATION type) { m_IOType = type; }
};
