#include "stdafx.h"
#include "MemoryManager.h"

namespace ServerEngine
{
	namespace System
	{
		//------------------------------------------------------------------------------------
		MemoryManager::MemoryManager()
			:m_TotalHeapSize(0),
			m_UsedHeapSize(0),
			m_pFirstAllocator(nullptr),
			m_pLastAllocator(nullptr),
			m_AllocatorCountNumber(0)
		{
		}

		//------------------------------------------------------------------------------------
		MemoryManager::~MemoryManager()
		{
		}

		//------------------------------------------------------------------------------------
		HRESULT MemoryManager::Init()
		{

			return S_OK;
		}

		void MemoryManager::Release()
		{
			DeleteAllAllocators();

		}

		//------------------------------------------------------------------------------------
		void MemoryManager::AddNewAllocator(E_Allocator e, const size_t AllocatorSize, const TCHAR * name, const size_t& classSize)
		{
			void* originalAllocatorPointer = nullptr;
			//void* alignedMemoryPointer = nullptr;
			Allocator* newAllocator = nullptr;
			Allocator* previousLastAllocator = nullptr;

			OutputDebugString(L"\n\n********New Add********\n");

			originalAllocatorPointer = malloc(AllocatorSize);
			m_TotalHeapSize += AllocatorSize;

			if (originalAllocatorPointer == nullptr)
			{
				ASSERT(0 && TEXT("Allocator 할당 실패"));
				return;
			}

			//정렬+요청 메모리 heap공간 생성, new placement 사용하여 생성자 불러옴
			//alignedMemoryPointer = originalAllocatorPointer;
			switch (e)
			{
			case E_LinearAllocator:

				newAllocator = new(originalAllocatorPointer) LinearAllocator(name, AllocatorSize, originalAllocatorPointer, originalAllocatorPointer);
				break;

			case E_StackAllocator:

				newAllocator = new(originalAllocatorPointer) StackAllocator(name, AllocatorSize, originalAllocatorPointer, originalAllocatorPointer);
				break;

			case E_FreeListAllocator:

				newAllocator = new(originalAllocatorPointer) FreeListAllocator(name, AllocatorSize, originalAllocatorPointer, originalAllocatorPointer);
				break;

			case E_PoolAllocator:

				newAllocator = new(originalAllocatorPointer) PoolAllocator(name, AllocatorSize, classSize, originalAllocatorPointer, originalAllocatorPointer);
				break;
			}

			m_HashMap.insert(make_pair(name, newAllocator));

			char string[500];
			sprintf_s(string, "First Pool: %p\n", m_pFirstAllocator);
			OutputDebugStringA(string);
			sprintf_s(string, "Last Pool: %p\n", m_pLastAllocator);
			OutputDebugStringA(string);
			sprintf_s(string, "Last Pool: %p\n", m_pLastAllocator);
			OutputDebugStringA(string);
			sprintf_s(string, "m_AllocatorCount: %d\n", (int)m_AllocatorCountNumber);
			OutputDebugStringA(string);

			return;
		}

		//------------------------------------------------------------------------------------
		void MemoryManager::DeleteAllocator(const TCHAR * name)
		{
			auto iter = m_HashMap.find(name);
			if (iter != m_HashMap.end())
			{
				iter->second->~Allocator();
				free(iter->second->GetOriginalAddress());
				m_HashMap.erase(iter);
			}
		}

		//------------------------------------------------------------------------------------
		void MemoryManager::DeleteAllAllocators()
		{
			for (auto iter = m_HashMap.begin(); iter != m_HashMap.end(); iter++)
			{
				iter->second->~Allocator();
				free(iter->second->GetOriginalAddress());
			}
		}

		//------------------------------------------------------------------------------------
		Allocator * MemoryManager::GetAllocator(const TCHAR * name)
		{
			auto iter = m_HashMap.find(name);
			if (iter != m_HashMap.end())
			{
				return iter->second;
			}

			return nullptr;
		}
	}
}
