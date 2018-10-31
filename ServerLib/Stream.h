#pragma once

class Stream
{
private:
	size_t m_Offset;
	size_t m_ReadPt;
	array <UCHAR, PAKCET_BUFF_SIZE> m_Stream;
	
public:
	Stream();
	Stream(UCHAR *stream, size_t size);
	~Stream();
	
	void Init();

	UCHAR* GetData() { return m_Stream.data(); }
	size_t GetSize() { return m_Offset; }
	void SetStream(UCHAR *stream, size_t size);

	// write
	//------------------------------------------------------------------------//
	bool CheckWriteBound(size_t len);

	void operator = (Stream &stream);
	template<class T>
	void operator << (const T &value);
	void operator << (const bool &value);
	void operator << (const INT8 &value);
	void operator << (const UINT8 &value);
	void operator << (const INT16 &value);
	void operator << (const UINT16 &value);
	void operator << (const INT32 &value);
	void operator << (const UINT32 &value);
	void operator << (const INT64 &value);
	void operator << (const UINT64 &value);

	void operator << (const std::vector<wstring> &value);

	void operator << (const string value);
	void operator << (const wstring value);

	// read
	//------------------------------------------------------------------------//
	bool CheckReadBound(size_t len);
	void Read(void *retVal, size_t len);

	template<class T>
	void operator >> (T *retVal);

	void operator >> (bool *retVal);
	void operator >> (INT8 *retVal);
	void operator >> (UINT8 *retVal);
	void operator >> (INT16 *retVal);
	void operator >> (UINT16 *retVal);
	void operator >> (INT32 *retVal);
	void operator >> (UINT32 *retVal);
	void operator >> (INT64 *retVal);
	void operator >> (UINT64 *retVal);

	void operator >> (std::vector<wstring> *retVal);

	void operator >> (string *retVal);
	void operator >> (wstring *retVal);
};
