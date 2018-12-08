/******************************************************************************
���ϸ�	: ClientSession.h
����		: ������ ���� Ŭ���̾�Ʈ�� �߻�ȭ��
******************************************************************************/
#pragma once

class LoginChattingClientSession : public ClientSession
{
private:
	LoginChattingClientSessionParser m_LoginChattingClientSessionParser;
	Player				m_PlayerData;

public:
	LoginChattingClientSession();
	~LoginChattingClientSession();

	static void* operator new(size_t allocSize)
	{
		if (m_AllocatorOn == FALSE)
		{
			 MEMORYMANAGER->AddNewAllocator(
				System::MemoryManager::E_PoolAllocator,
				allocSize * 10000, 
				TEXT("ClientSessionMemory"),
				allocSize
			);
			 m_AllocatorOn = TRUE;
		}
		
		return (LoginChattingClientSession*)(MEMORYMANAGER
			->GetAllocator(TEXT("ClientSessionMemory"))->Allocate(sizeof(LoginChattingClientSession),
				__alignof(LoginChattingClientSession)));
	}

	static void operator delete(void* deletepointer)
	{
		MEMORYMANAGER->GetAllocator(TEXT("ClientSessionMemory"))
			->Deallocate(deletepointer);
	}

	LoginChattingClientSessionParser* GetClientSessionParser() { return &m_LoginChattingClientSessionParser; }
	Player*			GetPlayerData() { return &m_PlayerData; }

	virtual HRESULT Init();

	/****************************************************************************
	�Լ���	: Release
	����		: Ŭ���̾�Ʈ ������ ����� ���� �����ϱ� ���� �Լ�
	*****************************************************************************/
	virtual void Release();


};
