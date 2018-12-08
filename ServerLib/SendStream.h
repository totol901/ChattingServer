/******************************************************************************
파일명	: SendStream.h
목적		: 송신용 스트림 객체
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
