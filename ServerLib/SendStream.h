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
			void write(string reval);
			void write(const bool& reVal);
			void write(const float& reVal);
			void write(const int& reVal);
			void write(const UINT& reVal);
			void write(const INT64& reVal);
			void write(const BYTE& reVal);
			void write(void* retVal, size_t len);
			void write(const E_PACKET_TYPE& retVal);
		};
	}
}
