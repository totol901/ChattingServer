/******************************************************************************
파일명	: SendStream.h
목적		: 송신용 스트림 객체
******************************************************************************/
#pragma once

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
	void write(string reVal);
	void write(void* retVal, size_t len);
};
