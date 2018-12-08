#include "stdafx.h"
#include "PoolAllocator.h"
//#include "GlobalManage.h"

namespace ServerEngine
{
	namespace System
	{
		PoolAllocator::PoolAllocator(const TCHAR * name, size_t totalsize, size_t blockSize, void * original, void * start)
			:Allocator(totalsize, original, start)
		{
			InitializeCriticalSection(&PoolAllocCS);
			m_BlockSize = blockSize;
			
			m_eKindOfAllocator = E_KindOfAllocator::E_POOL;

			int BlockCount = (int)((totalsize - sizeof(PoolAllocator)) / m_BlockSize);

			/*Free List(싱글 리스트)를 만들어서 초기화 함, 빈 블럭을 관리해주기 위함*/
			//m_ppFree_list에 첫번째 빈 블럭의 주소를 넣어주고
			m_ppFree_list = (void**)((size_t)start + sizeof(PoolAllocator));
			void** addr = m_ppFree_list;
			for (int i = 0; i < BlockCount; i++)
			{
				//빈 블럭에 다음(Next) 빈 블럭의 주소를 넣어줌
				*addr = (void*)((size_t)addr + m_BlockSize);

				addr = (void**)*addr;
			}
			*addr = nullptr;
		}

		PoolAllocator::~PoolAllocator()
		{
			DeleteCriticalSection(&PoolAllocCS);
			m_ppFree_list = nullptr;
		}

		void * PoolAllocator::Allocate(size_t size, size_t alignment)
		{
			ASSERT(size >= 4);
			ASSERT(alignment > 1);

			EnterCriticalSection(&PoolAllocCS);

			//공간 남았나 채크, 블럭에 들어가나 채크
			if (m_used_memory + alignment + size > m_totalSize
				|| size > m_BlockSize)
			{
				return nullptr;
			}

			/*첫번째 빈 블럭을 alignment 정렬하여 해당 주소를 return 해줌*/
			void* temp_addr = (void*)m_ppFree_list;
			size_t adjustment = 0;

			m_ppFree_list = (void**)*m_ppFree_list;

			adjustment = pointer_math::alignForwardAdjustment(temp_addr, alignment);

			m_used_memory += size;
			m_num_allocations++;

			LeaveCriticalSection(&PoolAllocCS);

			return (void*)((size_t)temp_addr + adjustment);
		}

		void PoolAllocator::Deallocate(void * p)
		{
			EnterCriticalSection(&PoolAllocCS);
			//해당 주소에 첫번째 빈 블럭의 주소를 저장하고, p를 첫번째 빈 블럭으로 바꿈
			void** temp_p = (void**)p;
			(*temp_p) = m_ppFree_list;
			m_ppFree_list = (void**)p;

			m_used_memory -= m_BlockSize;
			m_num_allocations--;
			LeaveCriticalSection(&PoolAllocCS);
		}
	}
}
