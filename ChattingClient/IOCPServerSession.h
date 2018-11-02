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
	�Լ���	: sendPacket
	����		: packet�����͸� ioData_ ���·� ���� �� send��
	���ϰ�	: ����
	�Ű�����	: Packet*
	*****************************************************************************/
	void SendPacket(T_PACKET packet);

	void Disconnect();

	void OnConnect(const char* serverIp, u_short serverPort);
	
	/****************************************************************************
	�Լ���	: onRecv
	����		: ������ �м�, Recv ������ ��Ŷ ��������
	���ϰ�	: Package*
	�Ű�����	: size_t
	*****************************************************************************/
	T_PACKET* OnRecv(size_t transferSize);
	
	/****************************************************************************
	�Լ���	: onSend
	����		: ��Ŷ ������ On�̸� ��Ŷ �۽���
	���ϰ�	: ����
	�Ű�����	: size_t
	*****************************************************************************/
	void OnSend(size_t transferSize);

	/****************************************************************************
	�Լ���	: RecvStandBy
	����		: ioData[IO_READ] ���� �ʱ�ȭ, IOCP�� recv �񵿱� ����
	���ϰ�	: ����
	�Ű�����	: ����
	*****************************************************************************/
	void RecvStandBy();
};

