#pragma once

class Allocator
{
public:
	enum E_KindOfAllocator
	{
		E_NONE,
		E_LINEAR,
		E_STACK,
		E_POOL,
		E_FreeList
	};

public:
	Allocator(size_t size, void* original, void* start)
		:m_pOriginalAddress(original),
		m_pStartAddress(start),
		m_totalSize(size),
		m_pPriorAllocator(nullptr),
		m_pNextAllocator(nullptr),
		//m_AllocatorID(0),
		m_used_memory(0),
		m_num_allocations(0)
	{
	}

	virtual ~Allocator()
	{
		ASSERT(m_num_allocations == 0 && m_used_memory == 0);
		m_pStartAddress = nullptr; 
		m_totalSize = 0;
	}

	virtual void* Allocate(size_t size, size_t alignment = 4) = 0;
	virtual void Deallocate(void* p) = 0;

	void SetPriorAllocator(Allocator* priorAllocator)	{ m_pPriorAllocator = priorAllocator; }
	void SetNextAllocator(Allocator* nextAllocator)		{ m_pNextAllocator = nextAllocator; }

	void* GetStartAddress() const					{ return m_pStartAddress; }
	void* GetOriginalAddress() const				{ return m_pOriginalAddress; }
	size_t GetSize() const							{ return m_totalSize; }
	size_t GetUsedMemory() const					{ return m_used_memory; }
	size_t GetNumAllocations() const				{ return m_num_allocations; }
	UINT GetAllocatorID() const						{ return m_AllocatorID; }
	Allocator* GetPriorAllocator() const			{ return m_pPriorAllocator; }
	Allocator* GetNextAllocator() const				{ return m_pNextAllocator; }
	const E_KindOfAllocator& GetKindOfAllocator()	{ return m_eKindOfAllocator; }
	
protected:
	E_KindOfAllocator m_eKindOfAllocator;
	UINT m_AllocatorID;
	size_t m_totalSize;
	size_t m_used_memory;
	size_t m_num_allocations;

	void* m_pOriginalAddress;;
	void* m_pStartAddress;
	Allocator* m_pPriorAllocator;
	Allocator* m_pNextAllocator;

};

//namespace allocator
//{
//	template <class T> T* allocateNew(Allocator& allocator)
//	{
//		return new (allocator.allocate(sizeof(T), __alignof(T))) T;
//	}
//
//	template <class T> T* allocateNew(Allocator& allocator, const T& t)
//	{
//		return new (allocator.allocate(sizeof(T), __alignof(T))) T(t);
//	}
//
//	template <class T> void deallocateDelete(Allocator& allocator, T& object)
//	{
//		object.~T();
//		allocator.deallocate(&object);
//	}
//
//	template <class T> T* allocateArray(Allocator& allocator, size_t length)
//	{
//		ASSERT(length != 0);
//		u8 headerSize = sizeof(size_t) / sizeof(T);
//
//		if (sizeof(size_t) % sizeof(T) > 0) headerSize += 1;
//
//		//Allocate extra space to store array length in the bytes before the array 
//		T* p = ((T*)allocator.allocate(sizeof(T)*(length + headerSize), __alignof(T))) + headerSize;
//		*(((size_t*)p) - 1) = length;
//
//		for (size_t i = 0; i < length; i++)
//			new (&p) T;
//
//		return p;
//	}
//
//	template <class T> void deallocateArray(Allocator& allocator, T* array)
//	{
//		ASSERT(array != nullptr);
//		size_t length = *(((size_t*)array) - 1);
//
//		for (size_t i = 0; i < length; i++) array.~T();
//
//		//Calculate how much extra memory was allocated to store the length before the array 
//		u8 headerSize = sizeof(size_t) / sizeof(T);
//		if (sizeof(size_t) % sizeof(T) > 0)
//			headerSize += 1;
//		allocator.deallocate(array - headerSize);
//	}
//};

namespace pointer_math
{
	inline void* alignForward(void* address, size_t alignment)
	{
		return (void*)((reinterpret_cast<size_t>(address) + static_cast<size_t>(alignment - 1)) & static_cast<size_t>(~(alignment - 1)));
	}

	inline size_t alignForwardAdjustment(void* address, size_t alignment)
	{
		size_t adjustment = alignment - (reinterpret_cast<size_t>(address) & (alignment - 1));

		if (adjustment == alignment) return 0;

		//already aligned 
		return adjustment;
	}

	inline size_t alignForwardAdjustmentWithHeader(void* address, size_t alignment, size_t headerSize)
	{
		size_t adjustment = alignForwardAdjustment(address, alignment);
		size_t neededSpace = headerSize;

		if (adjustment < neededSpace)
		{
			neededSpace -= adjustment;

			//Increase adjustment to fit header 
			adjustment += alignment * (neededSpace / alignment);

			if (neededSpace % alignment > 0) adjustment += alignment;
		}

		return adjustment;
	}
};
