#pragma once

namespace ServerEngine
{
	namespace System
	{
		class Thread
		{
		private:
			HANDLE m_hThread;
			UINT m_dThreadID;
			wstring m_strName;

			Util::Lock* m_CurrentLock;

		public:
			//Thread(_beginthreadex_proc_type func);
			Thread(void* arg, _beginthreadex_proc_type func);
			~Thread();

			const DWORD GetThreadID() const { return m_dThreadID; }
			const WCHAR* GetName() const { return m_strName.c_str(); }
			void SetCurrentLock(Util::Lock* lock) { m_CurrentLock = lock; }
			Util::Lock* GetCurrentLock() const { return m_CurrentLock; }
		};
	}
}
