/******************************************************************************
���ϸ�	: IOData.h
����		: IOCP�� �������忡 �� ��ü
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
	�Լ���	: clear
	����		: IoData Ŭ������ �����͸� 0���� �ʱ�ȭ��
	*****************************************************************************/
	void Clear();

	/****************************************************************************
	�Լ���	: needMoreIO
	����		: IO�� �� �ʿ����� �Ǻ���
	*****************************************************************************/
	bool NeedMoreIO(size_t transferSize);

	/****************************************************************************
	�Լ���	: setupTotalBytes
	����		: totalBytes_�� ������, ���� �������� ���� �� ��Ŷ ũ�� ��ȯ
	*****************************************************************************/
	int32_t SetTotalBytes();

	/****************************************************************************
	�Լ���	: wsabuf
	����		: ������� �� ����, ���� ���۸� ������
	*****************************************************************************/
	WSABUF GetCurrentWSABuf();

	/****************************************************************************
	�Լ���	: setData
	����		: buffer_�� stream�� ũ�⸦ �����, ���۸� �ڿ� �߰����ִ� ��Ŷ ����, totalBytes_ ����
	*****************************************************************************/
	bool SetData(T_PACKET packet);

public:
	//get�Լ�
	const size_t& GetTotalByte() const { return m_TotalBytes; }
	const IO_OPERATION& GetType() const { return m_IOType; }
	T_PACKET * const GetptPacket() { return &m_Packet; }
	LPWSAOVERLAPPED const GetptOverlapped() { return &m_Overlapped; }

	//set�Լ�
	void SetType(const IO_OPERATION& type) { m_IOType = type; }
};
