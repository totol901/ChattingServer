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

void RecvStream::wStringread(OUT string & retVal)
{
	//������ ���� �о��
	int stringlen = 0;
	memcpy_s(&stringlen, sizeof(stringlen),
		(void *)(stream.data() + readPt), sizeof(stringlen));
	readPt += sizeof(stringlen);

	//������ ���ۿ� �־���
	WCHAR* buff = new WCHAR[stringlen + 1];
	memcpy(buff,
		(void *)(stream.data() + readPt),
		sizeof(WCHAR)*(stringlen));
	readPt += stringlen* sizeof(WCHAR);

	buff[stringlen] = L'\0';

	char str[1024] = { 0, };
	StrConvW2A(buff, str, sizeof(str));

	//���۸� string����
	retVal.clear();
	retVal = str;

	SAFE_DELETE_ARRAY(buff);
}

void RecvStream::WStringStaticRead(OUT string & retVal)
{
	//������ ���� �о��
	int stringlen = 0;
	memcpy_s(&stringlen, sizeof(stringlen),
		(void *)(stream.data() + readPt), sizeof(stringlen));
	readPt += sizeof(stringlen);

	//������ ���ۿ� �־���
	WCHAR* buff = new WCHAR[stringlen + 1];
	memset(buff, 0, sizeof(WCHAR) *(stringlen + 1));
	memcpy(buff,
		(void *)(stream.data() + readPt),
		sizeof(WCHAR)*(stringlen));
	readPt += stringlen * sizeof(WCHAR);

	buff[stringlen] = L'\0';

	char str[1024] = { 0, };
	StrConvW2A(buff, str, sizeof(str));

	//���۸� string����
	retVal.clear();
	retVal = str;

	SAFE_DELETE_ARRAY(buff);
}
