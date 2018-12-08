#include "stdafx.h"
#include "FreeListAllocator.h"

namespace ServerEngine
{
	namespace System
	{
		FreeListAllocator::FreeListAllocator(const TCHAR * name, size_t size, void* original, void* start)
			:Allocator(size, original, start)
		{
			m_pFree_blocks = new((FreeBlock*)((size_t)start + sizeof(FreeListAllocator))) FreeBlock;
			m_pFree_blocks->size = size - sizeof(FreeListAllocator);
			m_pFree_blocks->next = nullptr;
			m_used_memory = sizeof(FreeListAllocator);

			m_eKindOfAllocator = E_KindOfAllocator::E_FreeList;
		}

		FreeListAllocator::~FreeListAllocator()
		{
			m_pFree_blocks = nullptr;
		}

		void* FreeListAllocator::Allocate(size_t size, size_t alignment)
		{
			ASSERT(size != 0 && alignment != 0);
			FreeBlock* prev_free_block = nullptr;
			FreeBlock* free_block = m_pFree_blocks;

			while (free_block != nullptr)
			{
				//AllocationHeader포함 메모리 정렬 시켜줌
				size_t adjustment = pointer_math::alignForwardAdjustmentWithHeader(free_block, alignment, sizeof(AllocationHeader));
				size_t total_size = size + adjustment;

				//이 free_block에 맞지 않으면 다음으로
				if (free_block->size < total_size)
				{
					prev_free_block = free_block;
					free_block = free_block->next;
					continue;
				}

				//static_assert(sizeof(AllocationHeader) >= sizeof(FreeBlock), "sizeof(AllocationHeader) < sizeof(FreeBlock)");

				//남은 메모리 크기가 FreeBlock보다 작으면
				if (free_block->size - total_size <= sizeof(AllocationHeader))
				{
					//total_size를 FreeBlock으로 키워줌
					total_size = free_block->size;

					//FreeBlock 리스트 링크 재설정
					if (prev_free_block != nullptr)
					{
						prev_free_block->next = free_block->next;
					}
					else
					{
						m_pFree_blocks = free_block->next;
					}
				}
				else
				{
					//할당된 메모리 뒤에 FreeBlock을 추가해줌
					FreeBlock* next_block = (FreeBlock*)(((size_t)free_block + total_size));

					next_block->size = free_block->size - total_size;
					next_block->next = free_block->next;

					//위에 추가해준 FreeBlcok을 링크 해줌
					if (prev_free_block != nullptr)
					{
						prev_free_block->next = next_block;
					}
					else
					{
						m_pFree_blocks = next_block;
					}
				}

				//AllocationHeader 할당
				void* aligned_address = (void*)((size_t)free_block + adjustment);
				AllocationHeader* header = (AllocationHeader*)((size_t)aligned_address - sizeof(AllocationHeader));
				header->size = total_size;
				header->adjustment = adjustment;
				m_used_memory += total_size;
				m_num_allocations++;

				ASSERT(pointer_math::alignForwardAdjustment((void*)aligned_address, alignment) == 0);

				return (void*)aligned_address;
			}

			//ASSERT(false && "Couldn't find free block large enough!"); 
			return nullptr;
		}

		void FreeListAllocator::Deallocate(void* p)
		{
			ASSERT(p != nullptr);

			//AllocationHeader 받아옴
			AllocationHeader* header = (AllocationHeader*)((size_t)p - sizeof(AllocationHeader));
			//블럭 시작점
			void* block_start = reinterpret_cast<void*>((size_t)(p)-header->adjustment);
			//블럭 사이즈
			size_t block_size = header->size;
			//블럭 끝점
			void* block_end = (void*)((size_t)block_start + block_size);
			FreeBlock* prev_free_block = nullptr;
			FreeBlock* free_block = m_pFree_blocks;

			//block_end 넘기기 직전의 free_block 설정 
			while (free_block != nullptr)
			{
				if ((void*)free_block >= block_end)
				{
					break;
				}

				prev_free_block = free_block;
				free_block = free_block->next;
			}

			//제일 마지막일 경우
			if (prev_free_block == nullptr)
			{
				prev_free_block = (FreeBlock*)block_start;
				prev_free_block->size = block_size;
				prev_free_block->next = m_pFree_blocks;
				m_pFree_blocks = prev_free_block;
			}
			//해당 블럭일 경우
			else if ((void*)((size_t)prev_free_block + prev_free_block->size) == block_start)
			{
				prev_free_block->size += block_size;
			}
			//이전일 경우
			else
			{
				FreeBlock* temp = (FreeBlock*)block_start;
				temp->size = block_size;
				temp->next = prev_free_block->next;
				prev_free_block->next = temp;
				prev_free_block = temp;
			}

			//해당 블럭의 크기를 키워줌
			if (free_block != nullptr && (void*)free_block == block_end)
			{
				prev_free_block->size += free_block->size;
				prev_free_block->next = free_block->next;
			}

			m_num_allocations--;
			m_used_memory -= block_size;
		}
	}
}
