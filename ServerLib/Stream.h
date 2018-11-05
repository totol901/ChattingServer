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
	~Stream();

	void clear();
	CHAR *data();
	size_t size();

	void operator = (Stream &stream);
	void set(CHAR *stream, size_t size);
};
