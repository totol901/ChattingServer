#pragma once
#include "stdafx.h"
#include "Stream.h"

namespace ServerEngine
{
	namespace NetworkSystem
	{
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
			memset(&stream, 0, sizeof(stream));
		}

		CHAR *Stream::data()
		{
			return stream.data();
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
			memcpy_s(this->stream.data(), stream.size(), (void *)data, size);
		}
	}
}
