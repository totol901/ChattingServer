#include "stdafx.h"
#include "IOData.h"

namespace ServerEngine
{
	namespace NetworkSystem
	{
		IOData::IOData()
			:m_IOType(IO_ERROR),
			m_TotalBytes(0),
			m_CurrentBytes(0)
		{
			memset(&m_Overlapped, 0, sizeof(m_Overlapped));
			m_Stream.clear();
		}

		IOData::~IOData()
		{
		}

		void IOData::Clear()
		{
			memset(&m_Stream, 0, sizeof(m_Stream));
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
				packetLen[0] = *(int32_t*)m_Stream.data();

				m_TotalBytes = (size_t)packetLen[0];
			}
			offset += sizeof(packetLen);

			return offset;
		}

		bool IOData::SetData(Stream& stream)
		{
			Clear();

			char* buf = m_Stream.data();
			int size[1] = { (int)stream.size() + (int)sizeof(int) };

			if (!memcpy_s(buf, m_Stream.GetBuf().max_size(),
				size, sizeof(int)) &&
				!memcpy_s(buf + sizeof(int), m_Stream.GetBuf().max_size() - sizeof(int),
				stream.data(), stream.size())
				)
			{
				m_Stream.Setoffset(size[0]);
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
			wsaBuf.buf = (char*)&m_Stream + m_CurrentBytes;
			wsaBuf.len = (ULONG)(m_TotalBytes - m_CurrentBytes);

			return wsaBuf;
		}
	}
}
