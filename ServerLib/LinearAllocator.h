#pragma once
#include "Allocator.h"
#include <memory>

class LinearAllocator : public Allocator
{
public:
	LinearAllocator(const TCHAR* name, size_t size, void* original, void* start);
	~LinearAllocator();

	void* Allocate(size_t size, size_t alignment) override;
	void Deallocate(void* p) override;
	void Clear();

private:
	//Ä«ÇÇ ¸·¾ÆÁÜ
	LinearAllocator& operator=(const LinearAllocator&) = delete;
	LinearAllocator(const LinearAllocator&) = delete;

private:
	void* m_pCurrent_Address;
};
