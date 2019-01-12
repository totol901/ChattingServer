#include "stdafx.h"
#include "ThreadManager.h"

namespace ServerEngine
{
	namespace System
	{
		ThreadManager::ThreadManager()
		{
		}

		ThreadManager::~ThreadManager()
		{
			for (auto iter : m_mapThreadPool)
			{
				SAFE_DELETE(iter.second);
			}
		}

		void ThreadManager::Push(Thread * thread)
		{
			m_mapThreadPool.insert(
				make_pair(
					thread->GetThreadID(),
					thread)
			);

			SLogPrintAtFile(
				L"* 스래드 생성 : id[0x%X] name[%s], pool size[%d]", 
				thread->GetThreadID(), 
				thread->GetName(), 
				m_mapThreadPool.size()
			);
		}

		void ThreadManager::Remove(DWORD threadID)
		{
			auto iter = m_mapThreadPool.find(threadID);
			if (iter == m_mapThreadPool.end())
			{
				return;
			}
			m_mapThreadPool.erase(threadID);
		}

		Thread * ThreadManager::FindThread(DWORD threadID)
		{
			if (m_mapThreadPool.empty())
			{
				return nullptr;
			}
			auto iter = m_mapThreadPool.find(threadID);
			if (iter == m_mapThreadPool.end())
			{
				return nullptr;
			}
			auto thread = iter->second;
			return thread;
		}


	}
}
