#pragma once

namespace ServerEngine
{
	namespace Util
	{

		class Lock
		{
		private:
			CRITICAL_SECTION m_CriticalSession;

			wstring m_Name;
			size_t m_ID;
			DWORD m_ThreadID;

			wstring m_LockingFile;
			int m_LockingLine;

		public:
			Lock(WCHAR* name);
			~Lock();

			/****************************************************************************
			�Լ���	: lock
			����		: ������ lock�� �ɰ�, �����̸�, ���� ��ȣ ����
			���ϰ�	: ����
			�Ű�����	: LPCWSTR, int
			*****************************************************************************/
			void lock(LPCWCHAR fileName, const int& lineNo);

			/****************************************************************************
			�Լ���	: unlock
			����		: ������ unlock�� ����
			���ϰ�	: ����
			�Ű�����	: ����
			*****************************************************************************/
			void unLock();

		public:
			void SetThreadID(const DWORD& ThreadID) { m_ThreadID = ThreadID; }
			const DWORD& GetThreadID() const { return m_ThreadID; }
			const WCHAR* GetName() { return m_Name.c_str(); }
		};
	}
}