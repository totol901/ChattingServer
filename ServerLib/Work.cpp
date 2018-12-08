#include "stdafx.h"
#include "Work.h"

namespace ServerEngine
{
	namespace System
	{
		BOOL Work::m_AllocatorOn = FALSE;

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
			if (m_AllocatorOn == FALSE)
			{
				MEMORYMANAGER->AddNewAllocator(
					MemoryManager::E_PoolAllocator,
					allocSize * 10000,
					TEXT("WorkMemory"),
					allocSize
				);
				m_AllocatorOn = TRUE;
			}

			return (Work*)(MEMORYMANAGER
				->GetAllocator(TEXT("WorkMemory"))->Allocate(sizeof(Work),
					__alignof(Work)));
		}

		void Work::operator delete(void * deletepointer)
		{
			MEMORYMANAGER->GetAllocator(TEXT("WorkMemory"))
				->Deallocate(deletepointer);
		}
	}
}