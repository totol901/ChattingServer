#include "stdafx.h"
#include "LinearAllocator.h"

namespace ServerEngine
{
	namespace System
	{
		//------------------------------------------------------------------------------------
		LinearAllocator::LinearAllocator(const TCHAR * name, size_t size, void* original, void * start)
			: Allocator(size, original, start),
			m_pCurrent_Address(reinterpret_cast<void*>(reinterpret_cast<size_t>(start) + sizeof(LinearAllocator)))
		{
			m_used_memory = sizeof(LinearAllocator);
			
			m_eKindOfAllocator = E_KindOfAllocator::E_LINEAR;
			ASSERT(size > 0);
		}

		//------------------------------------------------------------------------------------
		LinearAllocator::~LinearAllocator()
		{
			m_pCurrent_Address = nullptr;
		}

		//------------------------------------------------------------------------------------
		void* LinearAllocator::Allocate(size_t size, size_t alignment)
		{
			ASSERT(size != 0);
			//m_pCurrent_Address를 정렬하고 return, 필요한 size공간만큼 뒤로 이동 
			size_t adjustment = pointer_math::alignForwardAdjustment(m_pCurrent_Address, alignment);

			if (m_used_memory + adjustment + size > m_totalSize)
			{
				return nullptr;
			}

			void* aligned_address = reinterpret_cast<void*>(reinterpret_cast<size_t>(m_pCurrent_Address) + adjustment);
			m_pCurrent_Address = reinterpret_cast<void*>(reinterpret_cast<size_t>(aligned_address) + size);
			m_used_memory += size + adjustment;
			m_num_allocations++;

			//#ifdef _DEBUG
			//	char string[1024];
			//	sprintf_s(string, "-----------Linear Allocator allocate-----\n\n");
			//	OutputDebugStringA(string);
			//	sprintf_s(string, "m_used_memory: %d\n", m_used_memory);
			//	OutputDebugStringA(string);
			//	sprintf_s(string, "m_num_allocations : %d\n", m_num_allocations);
			//	OutputDebugStringA(string);
			//#endif

			return (void*)aligned_address;
		}

		//------------------------------------------------------------------------------------
		void LinearAllocator::Deallocate(void* p)
		{
			ASSERT(false && "Use clear() instead");
		}

		//------------------------------------------------------------------------------------
		void LinearAllocator::Clear()
		{
			m_num_allocations = 0;
			m_used_memory = 0;
			m_pCurrent_Address = reinterpret_cast<void*>(reinterpret_cast<size_t>(m_pStartAddress) + sizeof(LinearAllocator));

#ifdef _DEBUG
			char string[1024];
			sprintf_s(string, "-----------Linear Allocator allocate-----\n\n");
			OutputDebugStringA(string);
			sprintf_s(string, "m_used_memory: %zd\n", m_used_memory);
			OutputDebugStringA(string);
			sprintf_s(string, "m_num_allocations : %zd\n", m_num_allocations);
			OutputDebugStringA(string);
#endif
		}
	}
}
