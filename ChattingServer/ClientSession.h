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
	�Լ���	: sendPacket
	����		: packet�����͸� ioData_ ���·� ���� �� send��
	���ϰ�	: ����
	�Ű�����	: Packet*
	*****************************************************************************/
	void SendPacket(const T_PACKET& packet);

	void Disconnect();

	/****************************************************************************
	�Լ���	: onRecv
	����		: ������ �м�, Recv ������ ��Ŷ ��������
	���ϰ�	: Package*
	�Ű�����	: size_t
	*****************************************************************************/
	T_PACKET* OnRecv(const size_t& transferSize);

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
