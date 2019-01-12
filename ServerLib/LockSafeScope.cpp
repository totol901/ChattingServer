#include "stdafx.h"
#include "LockSafeScope.h"

namespace ServerEngine
{
	namespace Util
	{
		LockSafeScope::LockSafeScope(Lock* lock, LPCWCHAR fileName, int lineNo)
		{
			if (!lock)
			{
				return;
			}

			if (GlobalValue::GetInstance()->m_IsShutdown)
			{
				return;
			}

			m_pLock = lock;
			Lock *deadLock = LockManager::GetInstance()->CheckDeadLock(m_pLock);
			if (deadLock != nullptr)
			{
#ifdef _DEBUG
				SErrPrintAtFile(L"! [%s]lock and [%s]lock is dead detecting!", 
					deadLock->GetName(), 
					lock->GetName());
#else
				//데드락 해제
				deadLock->unLock();
				m_pLock->lock(fileName, lineNo);
				lock->SetThreadID(::GetCurrentThreadId());
				THREADPOOLMANAGER->FindThread(::GetCurrentThreadId())->SetCurrentLock(m_pLock);

				SLogPrintAtFile(L"!!! [%s]lock and [%s]lock is dead detecting!", 
					deadLock->GetName(),
					m_pLock->GetName());
#endif
				return;
			}

			m_pLock->lock(fileName, lineNo);
			lock->SetThreadID(::GetCurrentThreadId());
			THREADPOOLMANAGER->FindThread(::GetCurrentThreadId())->SetCurrentLock(m_pLock);
		}

		LockSafeScope::~LockSafeScope()
		{
			if (!m_pLock)
			{
				return;
			}

			if (GlobalValue::GetInstance()->m_IsShutdown)
			{
				return;
			}

			THREADPOOLMANAGER->FindThread(::GetCurrentThreadId())->SetCurrentLock(nullptr);
			m_pLock->unLock();
		}
	}
}
