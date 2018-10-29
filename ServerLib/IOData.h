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
	�Լ���	: clear
	����		: IoData Ŭ������ �����͸� 0���� �ʱ�ȭ��
	���ϰ�	: ����
	�Ű�����	: ����
	*****************************************************************************/
	void Clear();

	/****************************************************************************
	�Լ���	: needMoreIO
	����		: IO�� �� �ʿ����� �Ǻ���
	���ϰ�	: bool
	�Ű�����	: size_t
	*****************************************************************************/
	bool NeedMoreIO(size_t transferSize);

	/****************************************************************************
	�Լ���	: setupTotalBytes
	����		: totalBytes_�� ������, ���� �������� ���� �� ��Ŷ ũ�� ��ȯ
	���ϰ�	: int32_t
	�Ű�����	: ����
	*****************************************************************************/
	int32_t SetTotalBytes();

	/****************************************************************************
	�Լ���	: wsabuf
	����		: ������� �� ����, ���� ���۸� ������
	���ϰ�	: WSABUF
	�Ű�����	: ����
	*****************************************************************************/
	WSABUF GetCurrentWSABuf();

	/****************************************************************************
	�Լ���	: setData
	����		: buffer_�� stream�� ũ�⸦ �����, ���۸� �ڿ� �߰����ִ� ��Ŷ ����, totalBytes_ ����
	���ϰ�	: bool
	�Ű�����	: char*
	*****************************************************************************/
	bool SetData(T_PACKET stream);

public:
	//get�Լ�
	size_t GetTotalByte() { return m_TotalBytes; }
	IO_OPERATION GetType() { return m_IOType; }
	char * GetDataBuff() { return m_Buff; }
	LPWSAOVERLAPPED GetOverlapped() { return &m_Overlapped; }

	//set�Լ�
	void SetType(IO_OPERATION type) { m_IOType = type; }
};
