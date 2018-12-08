/******************************************************************************
���ϸ�	: RecvStream.h
����		: ���ſ� ��Ʈ�� ��ü
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
			void read(OUT string& retVal);
			void read(OUT void *retVal, size_t len);
			void wStringread(OUT wstring & retVal);

			void WStringStaticRead(OUT string & retVal);
		};
	}
}
