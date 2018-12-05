#include "stdafx.h"
#include "MemoryManager.h"

//------------------------------------------------------------------------------------
MemoryManager::MemoryManager()
	:m_TotalHeapSize(0),
	m_UsedHeapSize(0),
	m_pFirstAllocator(nullptr),
	m_pLastAllocator(nullptr),
	m_AllocatorCountNumber(0),
	m_pHash(nullptr)
{
}

//------------------------------------------------------------------------------------
MemoryManager::~MemoryManager()
{
}

//------------------------------------------------------------------------------------
HRESULT MemoryManager::Init()
{
	//프레임 마다 비워줄 LinearAllocator
	//10mb 할당
	//AddNewAllocator(E_LinearAllocator, 10 * 1024, TEXT("linear"));

	m_pHash = new c_Hash;

	return S_OK;
}

void MemoryManager::Release()
{
	DeleteAllAllocators();
	SAFE_DELETE(m_pHash);
}

//------------------------------------------------------------------------------------
UINT MemoryManager::AddNewAllocator(E_Allocator e, const size_t AllocatorSize, const TCHAR * name, const size_t& classSize)
{
	void* alignedAllocatorPointer = nullptr;
	void* originalAllocatorPointer = nullptr;
	void* alignedMemoryPointer = nullptr;
	Allocator* newAllocator = nullptr;
	Allocator* previousLastAllocator = nullptr;

	OutputDebugString(L"\n\n********New Add********\n");

	alignedAllocatorPointer = AlignedMalloc(AllocatorSize, 16);
	m_TotalHeapSize += AllocatorSize;

	if (alignedAllocatorPointer == nullptr)
	{
		//TOOD: malloc 에러, 로그 파일 추가, 종료
		return 0;
	}

	//정렬+요청 메모리 heap공간 생성, new placement 사용하여 생성자 불러옴
	switch (e)
	{
	case E_LinearAllocator:
		alignedMemoryPointer = reinterpret_cast<void*>(reinterpret_cast<size_t>(alignedAllocatorPointer));
		newAllocator = new(alignedAllocatorPointer) LinearAllocator(name, AllocatorSize, originalAllocatorPointer, alignedMemoryPointer);
	break;

	case E_StackAllocator:
		alignedMemoryPointer = reinterpret_cast<void*>(reinterpret_cast<size_t>(alignedAllocatorPointer));
		newAllocator = new(alignedAllocatorPointer) StackAllocator(name, AllocatorSize, originalAllocatorPointer, alignedMemoryPointer);
	break;

	case E_FreeListAllocator:
		alignedMemoryPointer = reinterpret_cast<void*>(reinterpret_cast<size_t>(alignedAllocatorPointer));
		newAllocator = new(alignedAllocatorPointer) FreeListAllocator(name, AllocatorSize, originalAllocatorPointer, alignedMemoryPointer);
		break;

	case E_PoolAllocator:
		alignedMemoryPointer = reinterpret_cast<void*>(reinterpret_cast<size_t>(alignedAllocatorPointer));
		newAllocator = new(alignedAllocatorPointer) PoolAllocator(name, AllocatorSize, classSize, originalAllocatorPointer, alignedMemoryPointer);
		break;
	}

	//매모리 풀의 내부 포인터 설정
	previousLastAllocator = m_pLastAllocator;
	if (m_pFirstAllocator == nullptr)
	{
		m_pFirstAllocator = newAllocator;
	}
	m_pLastAllocator = newAllocator;
	newAllocator->SetPriorAllocator(previousLastAllocator);
	newAllocator->SetNextAllocator(nullptr);
	++m_AllocatorCountNumber;
	if (previousLastAllocator != nullptr)
	{
		previousLastAllocator->SetNextAllocator(newAllocator);
	}

	char string[500];
	sprintf_s(string, "First Pool: %p\n", m_pFirstAllocator);
	OutputDebugStringA(string);
	sprintf_s(string, "Last Pool: %p\n", m_pLastAllocator);
	OutputDebugStringA(string);
	sprintf_s(string, "Last Pool: %p\n", m_pLastAllocator);
	OutputDebugStringA(string);
	sprintf_s(string, "m_AllocatorCount: %d\n", (int)m_AllocatorCountNumber);
	OutputDebugStringA(string);

	return newAllocator->GetAllocatorID();
}
//------------------------------------------------------------------------------------
void MemoryManager::DeleteAllocator(const int AllocaotrID)
{
	Allocator* dyingAllocator;
	void* memoryStart;

	dyingAllocator = m_pFirstAllocator;
	while (dyingAllocator != nullptr)
	{
		if (dyingAllocator->GetAllocatorID() == AllocaotrID)
		{
			if (dyingAllocator == m_pFirstAllocator)
			{
				m_pFirstAllocator = dyingAllocator->GetPriorAllocator();
			}
			if (dyingAllocator == m_pLastAllocator)
			{
				m_pLastAllocator = dyingAllocator->GetPriorAllocator();
			}
			if (dyingAllocator->GetPriorAllocator() != nullptr)
			{
				dyingAllocator->GetPriorAllocator()->SetNextAllocator(dyingAllocator->GetNextAllocator());
			}
			if (dyingAllocator->GetNextAllocator() != nullptr)
			{
				dyingAllocator->GetNextAllocator()->SetPriorAllocator(dyingAllocator->GetPriorAllocator());
			}

			memoryStart = dyingAllocator->GetOriginalAddress();
			dyingAllocator->~Allocator();
			free(memoryStart);
			return;
		}
		dyingAllocator = dyingAllocator->GetNextAllocator();
	}
}
//------------------------------------------------------------------------------------
void MemoryManager::DeleteAllocator(const TCHAR * name)
{
	unsigned int nameHashID = 0;
	
	nameHashID = m_pHash->Hash(name, sizeof(name));
	DeleteAllocator(nameHashID);
}
//------------------------------------------------------------------------------------
void MemoryManager::DeleteAllAllocators()
{
	while (m_pFirstAllocator != nullptr)
	{
		DeleteAllocator(m_pFirstAllocator->GetAllocatorID());
	}
}
//------------------------------------------------------------------------------------
Allocator * MemoryManager::GetAllocator(const TCHAR * name)
{
	unsigned int nameHashID = 0;

	nameHashID = m_pHash->Hash(name, sizeof(name));
	return GetAllocator(nameHashID);
}
//------------------------------------------------------------------------------------
Allocator * MemoryManager::GetAllocator(const int AllocaotrID)
{
	Allocator * Alloc = m_pFirstAllocator;
	while (Alloc != nullptr)
	{
		if (Alloc->GetAllocatorID() == AllocaotrID)
		{
			return Alloc;
		}
		Alloc = Alloc->GetNextAllocator();
	}

	ASSERT(0 && TEXT("Allocator 찾기 실패"));
	return nullptr;
}

//------------------------------------------------------------------------------------
void * MemoryManager::AlignedMalloc(const size_t requredBytes, const size_t alignemnet)
{
	return malloc(requredBytes);
}


//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//생성자를 불러오지 않은 new 연산자[가장 간단함]
//void* operator new(size_t size)
//{
//	return VMem::Alloc(size);
//}
////------------------------------------------------------------------------------------
//void* operator new(size_t size, const std::nothrow_t&)
//{
//	OutputDebugString(L"sizenothrow_t하학");
//	int i = 0;
//	return &i;
//}
////------------------------------------------------------------------------------------
//void* operator new[](size_t size)
//{
//	return VMem::Alloc(size);
//}
////------------------------------------------------------------------------------------
//void* operator new[](size_t size, const std::nothrow_t&)
//{
//	int i = 0; OutputDebugString(L"new[](size_t size, const std::nothrow_t&하학");
//	return &i;
//}
////------------------------------------------------------------------------------------
//void operator delete(void* p)
//{
//	VMem::Free(p);
//}
////------------------------------------------------------------------------------------
//void operator delete(void* p, std::nothrow_t&)
//{
//}
////------------------------------------------------------------------------------------
//void operator delete(void* p, size_t)
//{
//}
////------------------------------------------------------------------------------------
//void operator delete(void* p, size_t, std::nothrow_t&)
//{
//}
////------------------------------------------------------------------------------------
//void operator delete[](void* p)
//{
//	VMem::Free(p);
//}
////------------------------------------------------------------------------------------
//void operator delete[](void* p, std::nothrow_t&)
//{
//}
////------------------------------------------------------------------------------------
//void operator delete[](void* p, size_t)
//{
//}
////------------------------------------------------------------------------------------
//void operator delete[](void* p, size_t, std::nothrow_t&)
//{
//}
////------------------------------------------------------------------------------------
//void* operator new (size_t, int PoolID)
//{
//	int i = 0; 
//	OutputDebugString(L"하학");
//	return &i;
//}