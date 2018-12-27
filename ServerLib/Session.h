/******************************************************************************
���ϸ�	: Session.h
����		: ����, �θ� Ŭ����
******************************************************************************/
#pragma once

namespace ServerEngine
{
	namespace NetworkSystem
	{
		enum
		{
			SESSION_TYPE_NONE,
			SESSION_TYPE_TERMINAL,
			SESSION_TYPE_CLIENT,
			SESSION_TYPE_SERVER,
		};

		const int DeadMinTime = 1;

		class Session
		{
		protected:
			CRITICAL_SECTION m_HartBeatCS;
			CRITICAL_SECTION m_HeartBeatSendRecvCS;

			IOData		m_arrIOData[2];
			SOCKET		m_Socket;
			INT64		m_ID;
			int8_t		m_Type;
			SOCKADDR_IN m_ClientAddr;

			chrono::system_clock::time_point m_BeforeBeatTime;
			chrono::system_clock::time_point m_CurrentBeatTime;

			time_t m_SendTime;
			time_t m_RecvTime;
			time_t m_Ping;

			void Send(const WSABUF& wsaBuf);
			void Recv(const WSABUF& wsabuf);
			bool IsRecving(const size_t& transferSize);

		public:
			Session();
			virtual ~Session();

			//��Ʈ��Ʈ ���� �Լ���
			void UpdateHeartBeat();
			void SendHeartBeat();
			void RecvHeartBeat();
			bool ChackHeartBeat();

			const time_t& GetPing() const { return m_Ping; }

			//���� �ɼ�(���� ���� üũ) ����
			bool setSocketOpt();

			/****************************************************************************
			�Լ���	: PacketParsing
			����		: ��Ŷ�� �Ľ���
			*****************************************************************************/
			virtual const bool PacketParsing(Packet* const pakcet) = 0;

			/****************************************************************************
			�Լ���	: sendPacket
			����		: packet�����͸� ioData_ ���·� ���� �� send��
			*****************************************************************************/
			void SendPacket(Stream& stream);

			/****************************************************************************
			�Լ���	: onRecv
			����		: ������ �м�, Recv ������ ��Ŷ ��������
			*****************************************************************************/
			Packet* OnRecv(const size_t& transferSize);

			/****************************************************************************
			�Լ���	: onSend
			����		: ��Ŷ ������ On�̸� ��Ŷ �۽���
			*****************************************************************************/
			void OnSend(size_t transferSize);

			/****************************************************************************
			�Լ���	: RecvStandBy
			����		: ioData[IO_READ] ���� �ʱ�ȭ, IOCP�� recv �񵿱� ����
			*****************************************************************************/
			void RecvStandBy();

		public:
			//get�Լ�
			const SOCKET&		GetSocket() const { return m_Socket; }
			const INT64&		GetId() const { return m_ID; }
			const int8_t&		GetType() const { return m_Type; }
			const SOCKADDR_IN&	GetClientAddr() const { return m_ClientAddr; }

			//set�Լ�
			void SetSocketAddr(const SOCKADDR_IN& addr) { m_ClientAddr = addr; }
			void SetSocket(const SOCKET& socket) { m_Socket = socket; }
			void SetId(const INT64& id) { m_ID = id; }
			void SetType(const int8_t& type) { m_Type = type; }
		};
	}
}
