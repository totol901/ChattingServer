#pragma once

namespace ServerEngine
{
	namespace Util
	{
		//dump �Լ� ������
		typedef BOOL(WINAPI *WRITEDUMP)(
			_In_  HANDLE hProcess,
			_In_  DWORD ProcessId,
			_In_  HANDLE hFile,
			_In_  MINIDUMP_TYPE DumpType,
			_In_  PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
			_In_  PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
			_In_  PMINIDUMP_CALLBACK_INFORMATION CallbackParam
			);

		class MiniDump : public Singleton<MiniDump>
		{
			friend Singleton;
		private:
			MiniDump();
			~MiniDump();
			MiniDump(const MiniDump&) = delete;
			MiniDump(const MiniDump&&) = delete;
			MiniDump& operator = (const MiniDump&) = delete;
			MiniDump& operator = (const MiniDump&&) = delete;

		public:
			HRESULT Init();
			void Release();

			/****************************************************************************
			�Լ���	: execptionFilter
			����		: ���� �̴ϴ��� ������ ���� ���� ���� ���� �Լ�
			���ϰ�	: LONG WINAPI
			�Ű�����	: _EXCEPTION_POINTERS*
			*****************************************************************************/
			static LONG WINAPI execptionFilter(struct _EXCEPTION_POINTERS *exceptionInfo);
		};
	}
}