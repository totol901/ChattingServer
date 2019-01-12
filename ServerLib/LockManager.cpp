#include "stdafx.h"
#include "LockManager.h"

namespace ServerEngine
{
	namespace Util
	{
		LockManager::LockManager()
			:m_IDSeed(0)
		{
		}

		LockManager::~LockManager()
		{
		}

		Lock * LockManager::SearchLockCycle(Lock * newLock)
		{
			//list 따라 lock 이름을 비교해 본다.
			System::WorkThread *thread = THREADPOOLMANAGER->FindThread(::GetThreadId(NULL));
			if (!thread)
			{
				return nullptr;
			}
			vector<Lock *> trace;		// 데드락 탐지시, 걸린 락 stact 추척
			trace.push_back(newLock);

			Lock *deadLock = nullptr;
			while (true)
			{
				Lock *threadLock = thread->GetCurrentLock();
				if (threadLock == nullptr)
				{
					break;
				}
				if (threadLock->GetThreadID() == trace[0]->GetThreadID())
				{
					deadLock = threadLock;
					break;
				}
				trace.push_back(threadLock);
				thread = THREADPOOLMANAGER->FindThread(threadLock->GetThreadID());
				if (!thread)
				{
					break;
				}
			}
			trace.empty();

			return deadLock;
		}

		Lock * LockManager::CheckDeadLock(Lock * lock)
		{
			Lock *deadLock = this->SearchLockCycle(lock);
			if (deadLock)
			{
				return deadLock;
			}

			return nullptr;
		}

		const size_t LockManager::GeneralId()
		{
			size_t id = m_IDSeed++;
			return id;
		}
	}
}