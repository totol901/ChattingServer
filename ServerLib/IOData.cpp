#include "stdafx.h"
#include "IOData.h"

IOData::IOData()
	:m_IOType(IO_ERROR),
	m_TotalBytes(0),
	m_CurrentBytes(0)
{
	ZeroMemory(&m_Overlapped, sizeof(m_Overlapped));
	ZeroMemory(&m_Packet, sizeof(m_Packet));
}

IOData::~IOData()
{
}

void IOData::Clear()
{
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
		memcpy_s((void *)packetLen, sizeof(packetLen), 
			(void *)&m_Packet, sizeof(packetLen));
		
		m_TotalBytes = (size_t)packetLen[0];
	}
	offset += sizeof(packetLen);

	return offset;
}

bool IOData::SetData(T_PACKET stream)
{
	Clear();
	if (!memcpy_s((void *)&m_Packet, sizeof(m_Packet),
		(void *)&stream, sizeof(stream)))
	{
		return true;
	}
	else
	{
		return false;
	}
}

WSABUF IOData::GetCurrentWSABuf()
{
	WSABUF wsaBuf;
	wsaBuf.buf = (char*)&m_Packet + m_CurrentBytes;
	wsaBuf.len = (ULONG)(m_TotalBytes - m_CurrentBytes);

	return wsaBuf;
}
