#pragma once
#include "Allocator.h"
/*
**FreeListAllocator **

*/
class FreeListAllocator : public Allocator
{
public:
	/**  **/
	FreeListAllocator(const TCHAR * name, size_t size, void* original, void* start);
	~FreeListAllocator();
	FreeListAllocator& operator=(const FreeListAllocator&) = delete;

	void* Allocate(size_t size, size_t alignment) override;
	void Deallocate(void* p) override;

private:
	struct AllocationHeader 
	{ 
		size_t size; 
		size_t adjustment; 
	};
	struct FreeBlock 
	{ 
		size_t size; 
		FreeBlock* next; 
	};
	FreeListAllocator(const FreeListAllocator&);


	FreeBlock* m_pFree_blocks;
};

