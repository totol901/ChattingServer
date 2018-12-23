/******************************************************************************
���ϸ�	: Stream.h
����		: ��Ʈ�� ��ü, data�� ���� ó���ϱ� ����
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

			array <CHAR, PAKCET_BUFF_SIZE> stream;
		private:
			static BOOL m_AllocatorOn;

		public:
			Stream();
			Stream(CHAR *stream, size_t size);
			virtual ~Stream();

			void clear();
			CHAR *data();
			size_t size();

			void operator = (Stream &stream);
			void set(CHAR *stream, size_t size);

			static void* operator new(size_t allocSize);
			static void operator delete(void* deletepointer);
		};
	}
}
