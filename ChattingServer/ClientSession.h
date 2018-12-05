/******************************************************************************
���ϸ�	: ClientSession.h
����		: ������ ���� Ŭ���̾�Ʈ�� �߻�ȭ��
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
	�Լ���	: Clear
	����		: Ŭ���̾�Ʈ ������ ����� ���� �����ϱ� ���� �Լ�
	*****************************************************************************/
	void Clear();

	/****************************************************************************
	�Լ���	: PacketParsing
	����		: ��Ŷ�� �Ľ���
	*****************************************************************************/
	const bool PacketParsing(T_PACKET* const pakcet);

	/****************************************************************************
	�Լ���	: sendPacket
	����		: packet�����͸� ioData_ ���·� ���� �� send��
	*****************************************************************************/
	void SendPacket(const T_PACKET& packet);

	/****************************************************************************
	�Լ���	: onRecv
	����		: ������ �м�, Recv ������ ��Ŷ ��������
	*****************************************************************************/
	T_PACKET* OnRecv(const size_t& transferSize);

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
};
