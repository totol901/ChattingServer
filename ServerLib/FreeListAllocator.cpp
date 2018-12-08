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
				//AllocationHeader���� �޸� ���� ������
				size_t adjustment = pointer_math::alignForwardAdjustmentWithHeader(free_block, alignment, sizeof(AllocationHeader));
				size_t total_size = size + adjustment;

				//�� free_block�� ���� ������ ��������
				if (free_block->size < total_size)
				{
					prev_free_block = free_block;
					free_block = free_block->next;
					continue;
				}

				//static_assert(sizeof(AllocationHeader) >= sizeof(FreeBlock), "sizeof(AllocationHeader) < sizeof(FreeBlock)");

				//���� �޸� ũ�Ⱑ FreeBlock���� ������
				if (free_block->size - total_size <= sizeof(AllocationHeader))
				{
					//total_size�� FreeBlock���� Ű����
					total_size = free_block->size;

					//FreeBlock ����Ʈ ��ũ �缳��
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
					//�Ҵ�� �޸� �ڿ� FreeBlock�� �߰�����
					FreeBlock* next_block = (FreeBlock*)(((size_t)free_block + total_size));

					next_block->size = free_block->size - total_size;
					next_block->next = free_block->next;

					//���� �߰����� FreeBlcok�� ��ũ ����
					if (prev_free_block != nullptr)
					{
						prev_free_block->next = next_block;
					}
					else
					{
						m_pFree_blocks = next_block;
					}
				}

				//AllocationHeader �Ҵ�
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

			//AllocationHeader �޾ƿ�
			AllocationHeader* header = (AllocationHeader*)((size_t)p - sizeof(AllocationHeader));
			//�� ������
			void* block_start = reinterpret_cast<void*>((size_t)(p)-header->adjustment);
			//�� ������
			size_t block_size = header->size;
			//�� ����
			void* block_end = (void*)((size_t)block_start + block_size);
			FreeBlock* prev_free_block = nullptr;
			FreeBlock* free_block = m_pFree_blocks;

			//block_end �ѱ�� ������ free_block ���� 
			while (free_block != nullptr)
			{
				if ((void*)free_block >= block_end)
				{
					break;
				}

				prev_free_block = free_block;
				free_block = free_block->next;
			}

			//���� �������� ���
			if (prev_free_block == nullptr)
			{
				prev_free_block = (FreeBlock*)block_start;
				prev_free_block->size = block_size;
				prev_free_block->next = m_pFree_blocks;
				m_pFree_blocks = prev_free_block;
			}
			//�ش� ���� ���
			else if ((void*)((size_t)prev_free_block + prev_free_block->size) == block_start)
			{
				prev_free_block->size += block_size;
			}
			//������ ���
			else
			{
				FreeBlock* temp = (FreeBlock*)block_start;
				temp->size = block_size;
				temp->next = prev_free_block->next;
				prev_free_block->next = temp;
				prev_free_block = temp;
			}

			//�ش� ���� ũ�⸦ Ű����
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
