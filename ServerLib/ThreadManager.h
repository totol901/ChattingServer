#pragma once

namespace ServerEngine
{
	namespace System
	{
		class ThreadManager : public Singleton<ThreadManager>
		{
			friend Singleton;
		private:
			unordered_map<DWORD, Thread*> m_mapThreadPool;

			ThreadManager();
			~ThreadManager();
			ThreadManager(const ThreadManager&) = delete;
			ThreadManager(const ThreadManager&&) = delete;
			ThreadManager& operator = (const ThreadManager&) = delete;
			ThreadManager& operator = (const ThreadManager&&) = delete;
		
		public:
			void Push(Thread* thread);
			void Remove(DWORD threadID);
			Thread* FindThread(DWORD threadID);
		};
	}
}
