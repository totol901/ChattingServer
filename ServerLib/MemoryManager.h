#pragma once

/*
** MemoryManager **
Allocator�� �����ϴ� �Ŵ��� ��ü
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
	/*������Ʈ�� ���� Allocator�� �ʱ�ȭ */
	HRESULT Init();
	void Release();
	/*������Ʈ�� �߰��� Allocator ���� */
	UINT AddNewAllocator(E_Allocator e, const size_t AllocatorSize, const TCHAR* name, const size_t& classSize = 0);
	/*�ش� ID�� Allocator ����*/
	void DeleteAllocator(const int AllocaotrID);
	/*�ش� �̸��� Allocator ����*/
	void DeleteAllocator(const TCHAR* name);
	/*��� Allocator�� ����*/
	void DeleteAllAllocators();
	/*����ȭ�� �޸� ���� ����*/
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

//�۷ι� �������̵� new, delte opperator
//stl�� new �������̵�� �縳�Ҽ� ����.
//stl�Լ� �ȿ� ������ <new>�� ����ϴµ� �������̵� �ϸ�
//������ ���������� �����̴�. (������ �� �����ְų�, stl�� ������� �ʰų� ���� �ϳ�!)

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