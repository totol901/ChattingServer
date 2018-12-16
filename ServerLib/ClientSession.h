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
			static BOOL m_AllocatorOn;
			//ClientSessionParser	m_SessionParser;

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
			virtual void Release();

			virtual const bool PacketParsing(T_PACKET* const pakcet);
		};
	}
}
