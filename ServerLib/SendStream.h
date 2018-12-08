/******************************************************************************
���ϸ�	: SendStream.h
����		: �۽ſ� ��Ʈ�� ��ü
******************************************************************************/
#pragma once

namespace ServerEngine
{
	namespace NetworkSystem
	{
		class Stream;
		class SendStream : public Stream
		{
		public:
			SendStream();
			SendStream(CHAR *stream, size_t size);
			~SendStream();

			// write
			//------------------------------------------------------------------------//
			bool checkWriteBound(size_t len);
			void write(wstring reVal);
			void write(void* retVal, size_t len);
		};
	}
}
