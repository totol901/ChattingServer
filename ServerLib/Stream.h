#pragma once

class Stream
{
private:
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

	// write
	//------------------------------------------------------------------------//
	bool checkWriteBound(size_t len);
	void write(void* retVal, size_t len);

	// read
	//------------------------------------------------------------------------//
	bool checkReadBound(size_t len);
	void read(void *retVal, size_t len);
};
