#pragma once

class Player;
class ClientSessionParser;
class ClientSession : public Session
{
private:
	bool m_bConnected;

	void Send(const WSABUF& wsaBuf);
	void Recv(const WSABUF& wsabuf);
	bool IsRecving(const size_t& transferSize);
	
	ClientSessionParser* m_pSessionParser;
	Player* m_pPlayerData;

public:
	ClientSession();
	~ClientSession();

	const bool& IsConnected() { return m_bConnected; }
	void SetConnected(bool connected) { m_bConnected = connected;}
	Player* GetPlayerData() { return m_pPlayerData; }

	IOData* const GetptIOData(const int& type); const

	bool PacketParsing(T_PACKET* const pakcet);

	/****************************************************************************
	�Լ���	: sendPacket
	����		: packet�����͸� ioData_ ���·� ���� �� send��
	���ϰ�	: ����
	�Ű�����	: Packet*
	*****************************************************************************/
	void SendPacket(const T_PACKET& packet);

	//void Disconnect();

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
