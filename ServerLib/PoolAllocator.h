#pragma once
#include "Allocator.h"

class PoolAllocator : public Allocator
{
public:
	PoolAllocator(const TCHAR * name, size_t totalsize, size_t blockSize, void* original, void* start);
	~PoolAllocator();
	void* Allocate(size_t size, size_t alignment) override;
	void Deallocate(void* p) override;

private:
	CRITICAL_SECTION PoolAllocCS;
	//Ä«ÇÇ ¸·¾ÆÁÜ
	PoolAllocator(const PoolAllocator&);
	PoolAllocator& operator=(const PoolAllocator&); 
	
	size_t m_BlockSize;
	void** m_ppFree_list;
};
