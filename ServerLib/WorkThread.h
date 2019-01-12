#pragma once

namespace ServerEngine
{
	namespace System
	{
		class WorkThread
		{
		private:
			HANDLE m_hThread;
			UINT m_dThreadID;
			BOOL m_bIsOn;
			BOOL m_bIsWork;

			Work* m_pWork;

			HANDLE m_Event;

			Util::Lock* m_CurrentLock;

		public:
			WorkThread();
			~WorkThread();

			BOOL Init();
			void SignalEvent();
			static unsigned int WINAPI WorkRoop(LPVOID workThread);
			void HaveThreadWork(Work* work);

			const BOOL& IsOn() { return m_bIsOn; }
			const BOOL& IsWork() { return m_bIsWork; }
			void SetIsWork(const BOOL& iswork) { m_bIsWork = iswork; }

			HANDLE GetEvent() { return m_Event; }
			Work* GetWork() { return m_pWork; }
			void SetWork(Work* work) { m_pWork = work; }
			const DWORD GetThreadID() const { return m_dThreadID; }

			void SetCurrentLock(Util::Lock* lock) { m_CurrentLock = lock; }
			Util::Lock* GetCurrentLock() const { return m_CurrentLock; }
		};
	}
}
