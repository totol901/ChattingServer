#include "stdafx.h"
#include "SendStream.h"

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
	if (offset + len > sizeof(stream))
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
	memcpy_s((void *)(stream.data() + offset), stream.size() - offset, 
		(void *)&Stringlen, sizeof(Stringlen));
	offset += sizeof(Stringlen);

	//문장 넣어줌
	const wchar_t* str = reVal.c_str();
	memcpy_s((void *)(stream.data() + offset), stream.size() - offset,
		(void *)str, Stringlen * sizeof(wchar_t));
	offset += (Stringlen) * sizeof(wchar_t);
}

void SendStream::write(void * retVal, size_t len)
{
	memcpy_s((void *)(stream.data() + offset), stream.size() - offset, (const void *)retVal, len);
	offset += len;
}
