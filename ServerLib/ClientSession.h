/******************************************************************************
파일명	: ClientSession.h
목적		: 서버에 들어온 클라이언트를 추상화함
******************************************************************************/
#pragma once

namespace ServerEngine
{
	namespace NetworkSystem
	{
		class ClientSession : public Session
		{
		protected:
			//ClientSessionParser	m_SessionParser;

			void Send(const WSABUF& wsaBuf);
			void Recv(const WSABUF& wsabuf);
			bool IsRecving(const size_t& transferSize);

		public:
			ClientSession();
			virtual ~ClientSession();

			// TODO : 메모리풀 사용하려면 상속 받은 클래스에서 구현해야함
			//static void* operator new(size_t allocSize)
			//{
			//	if (m_AllocatorID == 0)
			//	{
			//		m_AllocatorID = MemoryManager::GetInstance()->AddNewAllocator(
			//			ServerEngine::MemoryManager::E_PoolAllocator,
			//			allocSize * 10000,
			//			TEXT("ClientSessionMemory"),
			//			allocSize
			//		);
			//	}
			//
			//	return (ClientSession*)(MemoryManager::GetInstance()
			//		->GetAllocator(m_AllocatorID)->Allocate(sizeof(ClientSession),
			//			__alignof(ClientSession)));
			//}
			//
			//static void operator delete(void* deletepointer)
			//{
			//	MemoryManager::GetInstance()->GetAllocator(m_AllocatorID)
			//		->Deallocate(deletepointer);
			//}

			//ClientSessionParser* GetClientSessionParser() { return &m_SessionParser; }
			//Player*			GetPlayerData() { return &m_PlayerData; }
			IOData* const	GetptIOData(const int& type);

			virtual HRESULT Init();
			/****************************************************************************
			함수명	: Release
			설명		: 클라이언트 세션을 지우기 전에 정리하기 위한 함수
			*****************************************************************************/
			virtual void Release() = 0;

			/****************************************************************************
			함수명	: PacketParsing
			설명		: 페킷을 파싱함
			*****************************************************************************/
			const bool PacketParsing(T_PACKET* const pakcet);

			/****************************************************************************
			함수명	: sendPacket
			설명		: packet데이터를 ioData_ 형태로 변형 후 send함
			*****************************************************************************/
			void SendPacket(const T_PACKET& packet);

			/****************************************************************************
			함수명	: onRecv
			설명		: 데이터 분석, Recv 상태인 패킷 리턴해줌
			*****************************************************************************/
			T_PACKET* OnRecv(const size_t& transferSize);

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
		};
	}
}
