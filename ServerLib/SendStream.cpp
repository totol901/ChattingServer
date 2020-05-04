#include "stdafx.h"
#include "SendStream.h"

namespace ServerEngine
{
	namespace NetworkSystem
	{
		SendStream::SendStream()
		{
		}

		SendStream::SendStream(CHAR * stream, size_t size)
			:Stream(stream, size)
		{
		}

		SendStream::~SendStream()
		{
		}

		// write
		//------------------------------------------------------------------------//
		bool SendStream::checkWriteBound(size_t len)
		{
			if (offset + len > sizeof(buffer))
			{
				SLogPrintAtFile("! 소켓 stream 버퍼 초과");
				ASSERT(FALSE);
				return false;
			}
			return true;
		}

		void SendStream::write(wstring  reVal)
		{
			//string 길이 넣어줌
			UINT Stringlen = (UINT)reVal.length();
			memcpy_s((void *)(buffer.data() + offset), buffer.size() - offset,
				(void *)&Stringlen, sizeof(Stringlen));
			offset += sizeof(Stringlen);

			//문장 넣어줌
			const wchar_t* str = reVal.c_str();
			memcpy_s((void *)(buffer.data() + offset), buffer.size() - offset,
				(void *)str, Stringlen * sizeof(wchar_t));
			offset += (Stringlen) * sizeof(wchar_t);
		}

		void SendStream::write(string reval)
		{
			//string 길이 넣어줌
			UINT Stringlen = (UINT)reval.length();
			memcpy_s((void *)(buffer.data() + offset), buffer.size() - offset,
				(void *)&Stringlen, sizeof(Stringlen));
			offset += sizeof(Stringlen);

			//문장 넣어줌
			const char* str = reval.c_str();
			memcpy_s((void *)(buffer.data() + offset), buffer.size() - offset,
				(void *)str, Stringlen);
			offset += (Stringlen);
		}

		void SendStream::write(const bool & reVal)
		{
			memcpy_s((void *)(buffer.data() + offset), buffer.size() - offset, (const void *)&reVal, sizeof(reVal));
			offset += sizeof(reVal);
		}

		void SendStream::write(const float & reVal)
		{
			memcpy_s((void *)(buffer.data() + offset), buffer.size() - offset, (const void *)&reVal, sizeof(reVal));
			offset += sizeof(reVal);
		}

		void SendStream::write(const int & reVal)
		{
			memcpy_s((void *)(buffer.data() + offset), buffer.size() - offset, (const void *)&reVal, sizeof(reVal));
			offset += sizeof(reVal);
		}

		void SendStream::write(const UINT & reVal)
		{
			memcpy_s((void *)(buffer.data() + offset), buffer.size() - offset, (const void *)&reVal, sizeof(reVal));
			offset += sizeof(reVal);
		}

		void SendStream::write(const INT64 & reVal)
		{
			memcpy_s((void *)(buffer.data() + offset), buffer.size() - offset, (const void *)&reVal, sizeof(reVal));
			offset += sizeof(reVal);
		}

		void SendStream::write(const UINT64 & reVal)
		{
			memcpy_s((void *)(buffer.data() + offset), buffer.size() - offset, (const void *)&reVal, sizeof(reVal));
			offset += sizeof(reVal);
		}

		void SendStream::write(const BYTE & reVal)
		{
			memcpy_s((void *)(buffer.data() + offset), buffer.size() - offset, (const void *)&reVal, sizeof(reVal));
			offset += sizeof(reVal);
		}

		void SendStream::write(void * retVal, size_t len)
		{
			memcpy_s((void *)(buffer.data() + offset), buffer.size() - offset, (const void *)retVal, len);
			offset += len;
		}

		void SendStream::write(const E_PACKET_TYPE& retVal)
		{
			memcpy_s((void *)(buffer.data() + offset), buffer.size() - offset, (const void *)&retVal, sizeof(retVal));
			offset += sizeof(retVal);
		}

	}
}
