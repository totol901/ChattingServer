#pragma once

class Stream;
class RecvStream : public Stream
{
public:
	RecvStream();
	RecvStream(CHAR *stream, size_t size);
	~RecvStream();

	// read
	//------------------------------------------------------------------------//
	bool checkReadBound(size_t len);
	void read(OUT string& retVal);
	void read(OUT void *retVal, size_t len);
};
