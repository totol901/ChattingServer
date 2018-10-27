#include "stdafx.h"
#include "Stream.h"

Stream::Stream()
{
	Init();
}

Stream::Stream(UCHAR *stream, size_t size)
{
	Init();
	SetStream(stream, size);
}

Stream::~Stream()
{
}

void Stream::Init()
{
	m_ReadPt = 0;
	m_Offset = 0;
	ZeroMemory(&m_Stream, sizeof(m_Stream));
}

void Stream::operator = (Stream &stream)
{
	SetStream(stream.GetData(), stream.GetSize());
}

void Stream::SetStream(UCHAR *data, size_t size)
{
	m_Offset = size;
	memcpy_s(this->m_Stream.data(), m_Stream.size(), (void *)data, size);
}

// write
//------------------------------------------------------------------------//
bool Stream::CheckWriteBound(size_t len)
{
	if (m_Offset + len > sizeof(m_Stream)) 
	{
		//SLog(L"! socket stream over.");
		//ASSERT(FALSE);
		return false;
	}
	return true;
}

#define STREAM_WRITE(value)						\
	INT32 size = sizeof(value);					\
	if (this->CheckWriteBound(size) == false)	\
	{											\
		return;									\
	}											\
	memcpy_s((void *)(m_Stream.data() + m_Offset),\
		m_Stream.size() - m_Offset,				\
		(const void *)&value, size);			\
	m_Offset += size;

template<class T>
void Stream::operator << (const T &value)
{
	STREAM_WRITE(value);
}
void Stream::operator << (const bool &value)
{
	STREAM_WRITE(value);
}
void Stream::operator << (const INT8 &value)
{
	STREAM_WRITE(value);
}
void Stream::operator << (const UINT8 &value)
{
	STREAM_WRITE(value);
}
void Stream::operator << (const INT16 &value)
{
	STREAM_WRITE(value);
}
void Stream::operator << (const UINT16 &value)
{
	STREAM_WRITE(value);
}
void Stream::operator << (const INT32 &value)
{
	STREAM_WRITE(value);
}
void Stream::operator << (const UINT32 &value)
{
	STREAM_WRITE(value);
}
void Stream::operator << (const INT64 &value)
{
	STREAM_WRITE(value);
}
void Stream::operator << (const UINT64 &value)
{
	STREAM_WRITE(value);
}
void Stream::operator << (const std::vector<wstring> &value)
{
	*this << value.size();
	for (auto i : value) 
	{
		*this << i;
	}
}
void Stream::operator << (const string value)
{
	*this << (int32_t)value.length();
	for (auto i : value) 
	{
		*this << i;
	}
}
void Stream::operator << (const wstring value)
{
	*this << (int32_t)(value.length());
	for (auto i : value) 
	{
		*this << i;
	}
}

// read
//------------------------------------------------------------------------//
bool Stream::CheckReadBound(size_t len)
{
	if (m_ReadPt + len > m_Offset) 
	{
		//SLog(L"! readOffset : %d, size: %d, totalOffset = %d", readPt_, len, offset_);
		//SLog(L"! socket stream has not more memory.");
		//ASSERT(FALSE);
		return false;
	}
	return true;
}

void Stream::Read(void *retVal, size_t len)
{
	memcpy_s(retVal, len, (void *)(m_Stream.data() + m_ReadPt), len);
	m_ReadPt += len;
}

#define STREAM_READ(type, retVal)				\
	size_t size = sizeof(type);					\
	if (this->CheckReadBound(size) == false)	\
	{											\
		return;									\
	}											\
	this->Read((void *)(retVal), size);	

template<class T>
void Stream::operator >> (T *retVal)
{
	STREAM_READ(T, retVal);
}
void Stream::operator >> (bool *retVal)
{
	STREAM_READ(bool, retVal);
}
void Stream::operator >> (INT8 *retVal)
{
	STREAM_READ(INT8, retVal);
}
void Stream::operator >> (UINT8 *retVal)
{
	STREAM_READ(UINT8, retVal);
}
void Stream::operator >> (INT16 *retVal)
{
	STREAM_READ(INT16, retVal);
}
void Stream::operator >> (UINT16 *retVal)
{
	STREAM_READ(UINT16, retVal);
}
void Stream::operator >> (INT32 *retVal)
{
	STREAM_READ(INT32, retVal);
}
void Stream::operator >> (UINT32 *retVal)
{
	STREAM_READ(UINT32, retVal);
}
void Stream::operator >> (INT64 *retVal)
{
	STREAM_READ(INT64, retVal);
}
void Stream::operator >> (UINT64 *retVal)
{
	STREAM_READ(UINT64, retVal);
}
void Stream::operator >> (std::vector<wstring> *retVal)
{
	size_t size;
	*this >> &size;

	for (size_t i = 0; i < size; ++i) 
	{
		wstring tmp;
		*this >> &tmp;
		retVal->push_back(tmp);
	}
}
void Stream::operator >> (string *retVal)
{
	INT32 size;
	*this >> &size;
	if (this->CheckReadBound(size) == false) 
	{
		return;
	}

	char *buf = new char[size + 1];
	this->Read((void *)(buf), size * sizeof(CHAR));
	buf[size] = '\0';

	retVal->clear();
	*retVal = buf;

	delete buf;
}
void Stream::operator >> (wstring *retVal)
{
	INT32 size;
	*this >> &size;
	if (this->CheckReadBound(size) == false) 
	{
		return;
	}

	WCHAR *buf = new WCHAR[size + 1];
	this->Read((void *)(buf), size * sizeof(WCHAR));
	buf[size] = '\0';

	retVal->clear();
	*retVal = buf;

	delete buf;
}
