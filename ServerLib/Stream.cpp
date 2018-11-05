#pragma once
#include "stdafx.h"
#include "Stream.h"

Stream::Stream()
{
	clear();
}

Stream::Stream(CHAR *stream, size_t size)
{
	clear();
	set(stream, size);
}

Stream::~Stream()
{
}

void Stream::clear()
{
	readPt = 0;
	offset = 0;
	memset(&stream, 0, sizeof(stream));
}

CHAR *Stream::data()
{
	return stream.data();
}

size_t Stream::size()
{
	return offset;
}

void Stream::operator = (Stream &stream)
{
	this->set(stream.data(), stream.size());
}

void Stream::set(CHAR *data, size_t size)
{
	this->offset = size;
	memcpy_s(this->stream.data(), stream.size(), (void *)data, size);
}

// write
//------------------------------------------------------------------------//
bool Stream::checkWriteBound(size_t len)
{
	if (offset + len > sizeof(stream)) 
	{
		//SLog(L"! socket stream over.");
		ASSERT(FALSE);
		return false;
	}
	return true;
}

void Stream::write(void * retVal, size_t len)
{
	memcpy_s((void *)(stream.data() + offset), stream.size() - offset, (const void *)retVal, len);
	offset += len;
}

// read
//------------------------------------------------------------------------//
bool Stream::checkReadBound(size_t len)
{
	if (readPt + len > offset) 
	{
		//SLog(L"! readOffset : %d, size: %d, totalOffset = %d", readPt_, len, offset_);
		//SLog(L"! socket stream has not more memory.");
		ASSERT(FALSE);
		return false;
	}
	return true;
}

void Stream::read(void *retVal, size_t len)
{
	memcpy_s(retVal, len, (void *)(stream.data() + readPt), len);
	readPt += len;
}


