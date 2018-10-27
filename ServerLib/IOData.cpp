#include "stdafx.h"
#include "IOData.h"

IOData::IOData()
	:m_IOType(IO_ERROR),
	m_TotalBytes(0),
	m_CurrentBytes(0)
{
	ZeroMemory(&m_Overlapped, sizeof(m_Overlapped));
	ZeroMemory(&m_Buff, sizeof(m_Buff));
}

IOData::~IOData()
{
}

void IOData::Clear()
{
	ZeroMemory(&m_Buff, sizeof(m_Buff));
	m_TotalBytes = 0;
	m_CurrentBytes = 0;
}

bool IOData::NeedMoreIO(size_t transferSize)
{
	m_CurrentBytes += transferSize;
	if (m_CurrentBytes < m_TotalBytes)
	{
		return true;
	}
	return false;
}

int32_t IOData::SetTotalBytes()
{
	int32_t offset = 0;
	int32_t packetLen[1] = { 0, };
	if (m_TotalBytes == 0)
	{
		memcpy_s((void *)packetLen, sizeof(packetLen), (void *)m_Buff, sizeof(packetLen));
		
		m_TotalBytes = (size_t)packetLen[0];
	}
	offset += sizeof(packetLen);

	return offset;
}

bool IOData::SetData(char* stream)
{
	Clear();

	memcpy_s(m_Buff, SOCKET_BUFF_SIZE, 
		(void *)stream, sizeof(stream));

	return true;
}


WSABUF IOData::GetCurrentWSABuf()
{
	WSABUF wsaBuf;
	wsaBuf.buf = m_Buff + m_CurrentBytes;
	wsaBuf.len = (ULONG)(m_TotalBytes - m_CurrentBytes);

	return wsaBuf;
}
