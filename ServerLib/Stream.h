/******************************************************************************
파일명	: Stream.h
목적		: 스트림 객체, data를 쉽게 처리하기 위함
******************************************************************************/
#pragma once

class Stream
{
protected:
	size_t offset;
	size_t readPt;

	array <CHAR, PAKCET_BUFF_SIZE> stream;

public:
	Stream();
	Stream(CHAR *stream, size_t size);
	virtual ~Stream();

	void clear();
	CHAR *data();
	size_t size();

	void operator = (Stream &stream);
	void set(CHAR *stream, size_t size);
};
