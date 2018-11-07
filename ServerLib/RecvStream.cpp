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
		SLogPrintAtFile("! ���� stream �޸� ����.");
		ASSERT(FALSE);
		return false;
	}
	return true;
}

void RecvStream::read(OUT string & retVal)
{
	//������ ���� �о��
	int stringlen = 0;
	memcpy_s(&stringlen, sizeof(stringlen),
		(void *)(stream.data() + readPt), sizeof(stringlen));
	readPt += sizeof(stringlen);

	//������ ���ۿ� �־���
	char* buff = new char[stringlen + 1];
	memcpy(buff, 
		(void *)(stream.data() + readPt), 
		(stringlen));
	readPt += stringlen;
	
	buff[stringlen] = '\0';

	//���۸� string����
	retVal.clear();
	retVal = buff;

	SAFE_DELETE_ARRAY(buff);
}

void RecvStream::read(OUT void * retVal, size_t len)
{
	memcpy_s(retVal, len, (void *)(stream.data() + readPt), len);
	readPt += len;
}
