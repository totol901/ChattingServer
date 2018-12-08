#include "stdafx.h"
#include "Packet.h"

namespace ServerEngine
{
	namespace NetworkSystem
	{
		BOOL T_PACKET::m_AllocatorOn = FALSE;

		T_PACKET::T_PACKET()
			:Size(0),
			type(PK_NONE)
		{
			memset(buff, 0, (PAKCET_BUFF_SIZE));
		}

		T_PACKET::T_PACKET(E_PACKET_TYPE _type)
		{
			Size = sizeof(*this);
			type = _type;
			memset(buff, 0, (PAKCET_BUFF_SIZE));
		}

		T_PACKET::~T_PACKET()
		{
		}

		void * T_PACKET::operator new(size_t allocSize)
		{
			if (m_AllocatorOn == FALSE)
			{
				MEMORYMANAGER->AddNewAllocator(
					System::MemoryManager::E_PoolAllocator,
					allocSize * 10000,
					TEXT("PacketMemory"),
					allocSize
				);
				m_AllocatorOn = TRUE;
			}

			return (T_PACKET*)(MEMORYMANAGER
				->GetAllocator(TEXT("PacketMemory"))->Allocate(sizeof(T_PACKET),
					__alignof(T_PACKET)));
		}

		void T_PACKET::operator delete(void * deletepointer)
		{
			MEMORYMANAGER->GetAllocator(TEXT("PacketMemory"))
				->Deallocate(deletepointer);
		}
	}
}