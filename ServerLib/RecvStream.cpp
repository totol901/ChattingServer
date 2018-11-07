#include "stdafx.h"
#include "RecvStream.h"

RecvStream::RecvStream()
{
}

RecvStream::RecvStream(CHAR * stream, size_t size)
	:Stream(stream, size)
{
}

RecvStream::~RecvStream()
{
}

// read
//------------------------------------------------------------------------//
bool RecvStream::checkReadBound(size_t len)
{
	if (readPt + len > offset)
	{
		SLogPrintAtFile("! readOffset : %d, size: %d, totalOffset = %d", readPt, len, offset);
		SLogPrintAtFile("! 소켓 stream 메모리 부족.");
		ASSERT(FALSE);
		return false;
	}
	return true;
}

void RecvStream::read(OUT string & retVal)
{
	//문장의 길이 읽어옴
	int stringlen = 0;
	memcpy_s(&stringlen, sizeof(stringlen),
		(void *)(stream.data() + readPt), sizeof(stringlen));
	readPt += sizeof(stringlen);

	//문장을 버퍼에 넣어줌
	char* buff = new char[stringlen + 1];
	memcpy(buff, 
		(void *)(stream.data() + readPt), 
		(stringlen));
	readPt += stringlen;
	
	buff[stringlen] = '\0';

	//버퍼를 string으로
	retVal.clear();
	retVal = buff;

	SAFE_DELETE_ARRAY(buff);
}

void RecvStream::read(OUT void * retVal, size_t len)
{
	memcpy_s(retVal, len, (void *)(stream.data() + readPt), len);
	readPt += len;
}
