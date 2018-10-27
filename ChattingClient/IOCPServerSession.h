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
	�Լ���	: onSend
	����		: ��Ŷ ������ On�̸� ��Ŷ �۽���
	���ϰ�	: ����
	�Ű�����	: size_t
	*****************************************************************************/
	void			OnSend(size_t transferSize);

	/****************************************************************************
	�Լ���	: sendPacket
	����		: packet�����͸� ioData_ ���·� ���� �� send��
	���ϰ�	: ����
	�Ű�����	: Packet*
	*****************************************************************************/
	void		    sendPacket();

	/****************************************************************************
	�Լ���	: onRecv
	����		: ������ �м�, Recv ������ ��Ŷ ��������
	���ϰ�	: Package*
	�Ű�����	: size_t
	*****************************************************************************/
	//Package*		onRecv(size_t transferSize);

	/****************************************************************************
	�Լ���	: RecvStandBy
	����		: ioData[IO_READ] ���� �ʱ�ȭ, IOCP�� recv �񵿱� ����
	���ϰ�	: ����
	�Ű�����	: ����
	*****************************************************************************/
	void			RecvStandBy();

};

