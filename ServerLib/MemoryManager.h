#pragma once

/*
** MemoryManager **
Allocator를 관리하는 매니저 객체
*/
class MemoryManager : public Singleton<MemoryManager>
{
	friend Singleton;
public:
	enum E_Allocator
	{
		E_LinearAllocator,
		E_StackAllocator,
		E_FreeListAllocator,
		E_PoolAllocator
	};

public:
	/*프로젝트에 쓰일 Allocator들 초기화 */
	HRESULT Init();
	void Release();
	/*프로젝트에 추가할 Allocator 생성 */
	UINT AddNewAllocator(E_Allocator e, const size_t AllocatorSize, const TCHAR* name, const size_t& classSize = 0);
	/*해당 ID의 Allocator 삭제*/
	void DeleteAllocator(const int AllocaotrID);
	/*해당 이름의 Allocator 삭제*/
	void DeleteAllocator(const TCHAR* name);
	/*모든 Allocator들 삭제*/
	void DeleteAllAllocators();
	/*단편화된 메모리 조각 모음*/
	//void DefragPool();
	//void QuaryPool();


	c_Hash* GetHashTable() { return m_pHash; }
	Allocator* GetAllocator(const TCHAR * name);
	Allocator* GetAllocator(const int AllocaotrID);

private:
	MemoryManager();
	~MemoryManager();
	MemoryManager(const MemoryManager&) = delete;
	MemoryManager(const MemoryManager&&) = delete;
	MemoryManager& operator = (const MemoryManager&) = delete;
	MemoryManager& operator = (const MemoryManager&&) = delete;

	size_t m_TotalHeapSize;
	size_t m_UsedHeapSize;
	size_t m_AllocatorCountNumber;

	Allocator* m_pFirstAllocator;
	Allocator* m_pLastAllocator;

	c_Hash* m_pHash;

	void* AlignedMalloc(const size_t requredBytes, const size_t alignemnet);
};

//글로벌 오버라이드 new, delte opperator
//stl과 new 오버라이드는 양립할수 없다.
//stl함수 안에 보통의 <new>를 사용하는데 오버라이딩 하면
//오류가 터져나오기 때문이다. (일일이 다 고쳐주거나, stl을 사용하지 않거나 둘중 하나!)

//void* operator new(size_t size);
//void* operator new(size_t size, const std::nothrow_t&);
//
//void* operator new[](size_t size);
//void* operator new[](size_t size, const std::nothrow_t&);
//
//void operator delete(void* p);
//void operator delete(void* p, std::nothrow_t&);
//void operator delete(void* p, size_t);
//void operator delete(void* p, size_t, std::nothrow_t&);
//void operator delete[](void* p);
//void operator delete[](void* p, std::nothrow_t&);
//void operator delete[](void* p, size_t);
//void operator delete[](void* p, size_t, std::nothrow_t&);
//
//void* operator new (size_t, int PoolID);