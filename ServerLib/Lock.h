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
			함수명	: lock
			설명		: 스레드 lock을 걸고, 파일이름, 라인 번호 저장
			리턴값	: 없음
			매개변수	: LPCWSTR, int
			*****************************************************************************/
			void lock(LPCWCHAR fileName, const int& lineNo);

			/****************************************************************************
			함수명	: unlock
			설명		: 스레드 unlock을 해줌
			리턴값	: 없음
			매개변수	: 없음
			*****************************************************************************/
			void unLock();

		public:
			void SetThreadID(const DWORD& ThreadID) { m_ThreadID = ThreadID; }
			const DWORD& GetThreadID() const { return m_ThreadID; }
			const WCHAR* GetName() { return m_Name.c_str(); }
		};
	}
}