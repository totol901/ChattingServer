/******************************************************************************
파일명	: RecvStream.h
목적		: 수신용 스트림 객체
******************************************************************************/
#pragma once

namespace ServerEngine
{
	namespace NetworkSystem
	{
		class Stream;
		class RecvStream : public Stream
		{
		public:
			RecvStream();
			RecvStream(CHAR *stream, size_t size);
			~RecvStream();

			// read
			//------------------------------------------------------------------------//
			bool checkReadBound(size_t len);
			void read(OUT string* retVal);
			void read(OUT wstring* retVal);
			void read(OUT bool* reVal);
			void read(OUT int* reVal);
			void read(OUT UINT* reVal);
			void read(OUT BYTE* reVal);
			void read(OUT float* reVal);
			void read(OUT INT64* reVal);
			void read(OUT UINT64* reVal);
			void read(OUT void *retVal, size_t len);
			

			//void WStringStaticRead(OUT string & retVal);
		};
	}
}
