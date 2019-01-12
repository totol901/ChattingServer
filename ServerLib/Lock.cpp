#include "stdafx.h"
#include "Lock.h"

namespace ServerEngine
{
	namespace Util
	{
		const int INVALID_LINE = -1;

		Lock::Lock(WCHAR* name)
			:m_Name(name),
			m_LockingLine(INVALID_LINE)
		{
			m_LockingFile.clear();
			InitializeCriticalSection(&m_CriticalSession);
		}

		Lock::~Lock()
		{
			DeleteCriticalSection(&m_CriticalSession);
			m_Name.clear();
		}

		void Lock::lock(LPCWCHAR fileName, const int& lineNo)
		{
			EnterCriticalSection(&m_CriticalSession);
			m_LockingFile = fileName;
			m_LockingLine = lineNo;
		}

		void Lock::unLock()
		{
			LeaveCriticalSection(&m_CriticalSession);

			m_LockingFile.clear();
			m_LockingLine = INVALID_LINE;
		}

	}
}