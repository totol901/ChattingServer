#include "stdafx.h"
#include "Work.h"

UINT Work::m_AllocatorID = 0;

Work::Work()
	:function(nullptr)
{
}

Work::~Work()
{
}

void Work::DoWork()
{
	function();
}

void * Work::operator new(size_t allocSize)
{
	
	if (m_AllocatorID == 0)
	{
		m_AllocatorID = MEMORYMANAGER->AddNewAllocator(
			MemoryManager::E_PoolAllocator,
			allocSize * 10000,
			TEXT("WorkMemory"),
			allocSize
		);
	}

	return (Work*)(MEMORYMANAGER
		->GetAllocator(m_AllocatorID)->Allocate(sizeof(Work),
			__alignof(Work)));
}

void Work::operator delete(void * deletepointer)
{
	MEMORYMANAGER->GetAllocator(m_AllocatorID)
		->Deallocate(deletepointer);
}
