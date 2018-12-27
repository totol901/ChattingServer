/******************************************************************************
파일명	: Stream.h
목적		: 스트림 객체, data를 쉽게 처리하기 위함
******************************************************************************/
#pragma once

namespace ServerEngine
{
	namespace NetworkSystem
	{
		class Stream
		{
		protected:
			size_t offset;
			size_t readPt;

			array <CHAR, PAKCET_BUFF_SIZE> buffer;
		protected:
			static BOOL m_AllocatorOn;

		public:
			Stream();
			Stream(CHAR *stream, size_t size);
			virtual ~Stream();

			E_PACKET_TYPE& GetType()
			{
				return *(E_PACKET_TYPE*)(buffer.data() + sizeof(int));
			}

			void clear();
			CHAR *data();

			size_t size();

			void operator = (Stream &stream);
			void set(CHAR *stream, size_t size);
			void SetreadPt(const size_t& rp){ readPt = rp; }
			void Setoffset(const size_t& ot){offset = ot;}

			array <CHAR, PAKCET_BUFF_SIZE>& GetBuf(){return buffer;}

			static void* operator new(size_t allocSize);
			static void operator delete(void* deletepointer);
		};
	}
}
