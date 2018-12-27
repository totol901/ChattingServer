/******************************************************************************
파일명	: Session.h
목적		: 세션, 부모 클래스
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

			//하트비트 관련 함수들
			void UpdateHeartBeat();
			void SendHeartBeat();
			void RecvHeartBeat();
			bool ChackHeartBeat();

			const time_t& GetPing() const { return m_Ping; }

			//소켓 옵션(좀비 소켓 체크) 설정
			bool setSocketOpt();

			/****************************************************************************
			함수명	: PacketParsing
			설명		: 페킷을 파싱함
			*****************************************************************************/
			virtual const bool PacketParsing(Packet* const pakcet) = 0;

			/****************************************************************************
			함수명	: sendPacket
			설명		: packet데이터를 ioData_ 형태로 변형 후 send함
			*****************************************************************************/
			void SendPacket(Stream& stream);

			/****************************************************************************
			함수명	: onRecv
			설명		: 데이터 분석, Recv 상태인 패킷 리턴해줌
			*****************************************************************************/
			Packet* OnRecv(const size_t& transferSize);

			/****************************************************************************
			함수명	: onSend
			설명		: 페킷 보내기 On이면 패킷 송신함
			*****************************************************************************/
			void OnSend(size_t transferSize);

			/****************************************************************************
			함수명	: RecvStandBy
			설명		: ioData[IO_READ] 버퍼 초기화, IOCP의 recv 비동기 실행
			*****************************************************************************/
			void RecvStandBy();

		public:
			//get함수
			const SOCKET&		GetSocket() const { return m_Socket; }
			const INT64&		GetId() const { return m_ID; }
			const int8_t&		GetType() const { return m_Type; }
			const SOCKADDR_IN&	GetClientAddr() const { return m_ClientAddr; }

			//set함수
			void SetSocketAddr(const SOCKADDR_IN& addr) { m_ClientAddr = addr; }
			void SetSocket(const SOCKET& socket) { m_Socket = socket; }
			void SetId(const INT64& id) { m_ID = id; }
			void SetType(const int8_t& type) { m_Type = type; }
		};
	}
}
