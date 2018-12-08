/******************************************************************************
파일명	: ClientSession.h
목적		: 서버에 들어온 클라이언트를 추상화함
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
	함수명	: Release
	설명		: 클라이언트 세션을 지우기 전에 정리하기 위한 함수
	*****************************************************************************/
	virtual void Release();


};
