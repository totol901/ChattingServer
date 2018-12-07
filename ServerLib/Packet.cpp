#include "stdafx.h"
#include "Packet.h"

UINT T_PACKET::m_AllocatorID = 0;

T_PACKET::T_PACKET()
	:Size(0),
	type(PK_NONE)
{
	memset(buff, 0, (PAKCET_BUFF_SIZE));
}

T_PACKET::T_PACKET(E_PACKET_TYPE _type)
{
	Size = sizeof(*this);
	type = _type;
	memset(buff, 0, (PAKCET_BUFF_SIZE));
}

T_PACKET::~T_PACKET()
{
}

void * T_PACKET::operator new(size_t allocSize)
{
	if (m_AllocatorID == 0)
	{
		m_AllocatorID = MEMORYMANAGER->AddNewAllocator(
			MemoryManager::E_PoolAllocator,
			allocSize * 10000,
			TEXT("PacketMemory"),
			allocSize
		);
	}

	return (T_PACKET*)(MEMORYMANAGER
		->GetAllocator(m_AllocatorID)->Allocate(sizeof(T_PACKET),
			__alignof(T_PACKET)));
}

void T_PACKET::operator delete(void * deletepointer)
{
	MEMORYMANAGER->GetAllocator(m_AllocatorID)
		->Deallocate(deletepointer);
}
