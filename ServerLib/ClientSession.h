/******************************************************************************
���ϸ�	: ClientSession.h
����		: ������ ���� Ŭ���̾�Ʈ�� �߻�ȭ��
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

			// TODO : �޸�Ǯ ����Ϸ��� ��� ���� Ŭ�������� �����ؾ���
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
			�Լ���	: Release
			����		: Ŭ���̾�Ʈ ������ ����� ���� �����ϱ� ���� �Լ�
			*****************************************************************************/
			virtual void Release();

			virtual const bool PacketParsing(T_PACKET* const pakcet);
		};
	}
}
