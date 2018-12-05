#pragma once
#include "Allocator.h" 

class StackAllocator : public Allocator
{
public:
	StackAllocator(const TCHAR * name, size_t size, void* original, void* start);
	~StackAllocator();

	void* Allocate(size_t size, size_t alignment) override;
	void Deallocate(void* p) override;
	void Clear();

private:
	//Ä«ÇÇ ¸·¾ÆÁÜ
	StackAllocator(const StackAllocator&);
	StackAllocator& operator=(const StackAllocator&);

	struct AllocationHeader
	{
		BYTE adjustment;
	};

	void* m_pPrev_Address;
	void* m_pCurrent_Address;
	size_t m_Prev_Size;

};