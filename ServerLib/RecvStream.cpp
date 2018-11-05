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
		SLogPrintAtFile("! 家南 stream 皋葛府 何练.");
		ASSERT(FALSE);
		return false;
	}
	return true;
}

void RecvStream::read(void *retVal, size_t len)
{
	memcpy_s(retVal, len, (void *)(stream.data() + readPt), len);
	readPt += len;
}
