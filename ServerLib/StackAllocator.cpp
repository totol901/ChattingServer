#include "stdafx.h"
#include "StackAllocator.h"


//------------------------------------------------------------------------------------
StackAllocator::StackAllocator(const TCHAR * name, size_t size, void* original, void* start)
	: Allocator(size, original, start)
	, m_Prev_Size(0),
	m_pPrev_Address(nullptr),
	m_pCurrent_Address(reinterpret_cast<void*>(reinterpret_cast<size_t>(start) + sizeof(StackAllocator)))
{
	m_used_memory = sizeof(StackAllocator);
	m_AllocatorID = MEMORYMANAGER->GetHashTable()->Hash(name, sizeof(name));
	m_eKindOfAllocator = E_KindOfAllocator::E_STACK;
	ASSERT(size > 0);
}
//------------------------------------------------------------------------------------
StackAllocator::~StackAllocator()
{
	m_pCurrent_Address = nullptr;
	m_pPrev_Address = nullptr;
	m_pStartAddress = nullptr;
}
//------------------------------------------------------------------------------------
void* StackAllocator::Allocate(size_t size, size_t alignment)
{
	ASSERT(size != 0);
	ASSERT(alignment > 1);

	//공간 남았나 채크
	if (m_used_memory + alignment + size > m_totalSize)
	{
		return nullptr;
	}

	void* original_Address = m_pCurrent_Address;
	BYTE adjustment = 0;
	AllocationHeader* header = nullptr;
	void* aligned_address = nullptr;

	m_Prev_Size = size + alignment;
	
	//다음 스택 시작점 가리킴
	m_pCurrent_Address = reinterpret_cast<void*>(reinterpret_cast<size_t>(m_pCurrent_Address) + m_Prev_Size);
	//정렬 바이트 구해줌
	adjustment = static_cast<BYTE>(pointer_math::alignForwardAdjustment(original_Address, alignment));

	//정렬된 주소 구해줌
	aligned_address = reinterpret_cast<void*>(reinterpret_cast<size_t>(original_Address) + adjustment);
	//헤더에 정렬 바이트 넣어줌
	header = reinterpret_cast<AllocationHeader*>(reinterpret_cast<size_t>(aligned_address) - sizeof(BYTE));
	header->adjustment = adjustment;

	m_used_memory += size + alignment;
	m_num_allocations++;

#ifdef _DEBUG
	char string[1024];
	sprintf_s(string, "-----------stack Allocator allocate-----\n\n");
	OutputDebugStringA(string);
	sprintf_s(string, "m_used_memory: %zd\n", m_used_memory);
	OutputDebugStringA(string);
	sprintf_s(string, "m_num_allocations : %zd\n", m_num_allocations);
	OutputDebugStringA(string);
#endif
	m_pPrev_Address = aligned_address;

	return aligned_address;
}

//------------------------------------------------------------------------------------
void StackAllocator::Deallocate(void* p)
{
	AllocationHeader* header = reinterpret_cast<AllocationHeader*>(reinterpret_cast<size_t>(p) - sizeof(BYTE));
	size_t erase_size = reinterpret_cast<size_t>(m_pCurrent_Address) - reinterpret_cast<size_t>(p) + header->adjustment;
	m_used_memory -= erase_size;
	
	m_pCurrent_Address = reinterpret_cast<void*>(reinterpret_cast<size_t>(m_pCurrent_Address) - erase_size);

	m_num_allocations--;

#ifdef _DEBUG
	char string[1024];
	sprintf_s(string, "-----------stack Allocator allocate-----\n\n");
	OutputDebugStringA(string);
	sprintf_s(string, "m_used_memory: %zd\n", m_used_memory);
	OutputDebugStringA(string);
	sprintf_s(string, "m_num_allocations : %zd\n", m_num_allocations);
	OutputDebugStringA(string);
#endif
}

//------------------------------------------------------------------------------------
void StackAllocator::Clear()
{
	m_Prev_Size = 0;
	m_pCurrent_Address = reinterpret_cast<void*>(reinterpret_cast<size_t>(m_pStartAddress) + sizeof(StackAllocator));
	m_pPrev_Address = nullptr;
	m_num_allocations = 0;
	m_used_memory = 0;
}
