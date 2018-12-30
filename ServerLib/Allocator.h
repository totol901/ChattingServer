#pragma once

namespace ServerEngine
{
	namespace System
	{
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

			void* GetStartAddress() const { return m_pStartAddress; }
			void* GetOriginalAddress() const { return m_pOriginalAddress; }
			size_t GetSize() const { return m_totalSize; }
			size_t GetUsedMemory() const { return m_used_memory; }
			size_t GetNumAllocations() const { return m_num_allocations; }
			
			const E_KindOfAllocator& GetKindOfAllocator() { return m_eKindOfAllocator; }
			size_t GetUsedMemoryPercentage() { return (size_t)((GetUsedMemory() / (float)GetSize()) * 100); }

		protected:
			E_KindOfAllocator m_eKindOfAllocator;

			size_t m_totalSize;
			size_t m_used_memory;
			size_t m_num_allocations;

			void* m_pOriginalAddress;;
			void* m_pStartAddress;

		};

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
	}
}