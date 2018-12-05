/******************************************************************************
파일명	: ClientSession.h
목적		: 서버에 들어온 클라이언트를 추상화함
******************************************************************************/
#pragma once

class ClientSession : public Session
{
private:
	static UINT m_AllocatorID;
	void Send(const WSABUF& wsaBuf);
	void Recv(const WSABUF& wsabuf);
	bool IsRecving(const size_t& transferSize);
	
	ClientSessionParser	m_SessionParser;
	Player				m_PlayerData;

public:
	ClientSession();
	~ClientSession();

	static void* operator new(size_t allocSize)
	{
		
		if (m_AllocatorID == 0)
		{
			m_AllocatorID = MEMORYMANAGER->AddNewAllocator(
				MemoryManager::E_PoolAllocator, 
				allocSize * 10000, 
				TEXT("ClientSessionMemory"),
				allocSize
			);
		}
		
		return (ClientSession*)(MEMORYMANAGER
			->GetAllocator(m_AllocatorID)->Allocate(sizeof(ClientSession),
				__alignof(ClientSession)));
	}

	static void operator delete(void* deletepointer)
	{
		
		MEMORYMANAGER->GetAllocator(m_AllocatorID)
			->Deallocate(deletepointer);
		
	}

	Player*			GetPlayerData() { return &m_PlayerData; }
	IOData* const	GetptIOData(const int& type);

	/****************************************************************************
	함수명	: Clear
	설명		: 클라이언트 세션을 지우기 전에 정리하기 위한 함수
	*****************************************************************************/
	void Clear();

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
