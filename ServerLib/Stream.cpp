#pragma once
#include "stdafx.h"
#include "Stream.h"

namespace ServerEngine
{
	namespace NetworkSystem
	{
		BOOL Stream::m_AllocatorOn = FALSE;

		void * Stream::operator new(size_t allocSize)
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

			return (Stream*)(MEMORYMANAGER->
				GetAllocator(TEXT("PacketMemory"))->Allocate(sizeof(Stream),
					__alignof(Stream)));
		}
		
		void Stream::operator delete(void * deletepointer)
		{
			MEMORYMANAGER->GetAllocator(TEXT("PacketMemory"))
				->Deallocate(deletepointer);
		}

		Stream::Stream()
		{
			clear();
		}

		Stream::Stream(CHAR *stream, size_t size)
		{
			clear();
			set(stream, size);
		}

		Stream::~Stream()
		{
		}

		void Stream::clear()
		{
			offset = 0;
			readPt = 0;
			memset(&buffer, 0, sizeof(buffer));
		}

		CHAR *Stream::data()
		{
			return buffer.data();
		}

		size_t Stream::size()
		{
			return offset;
		}

		void Stream::operator = (Stream &stream)
		{
			this->set(stream.data(), stream.size());
		}

		void Stream::set(CHAR *data, size_t size)
		{
			this->offset = size;
			memcpy_s(this->buffer.data(), buffer.size(), (void *)data, size);
		}

	}
}
