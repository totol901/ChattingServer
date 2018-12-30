#pragma once

namespace ServerEngine
{
	namespace Util
	{
		//dump 함수 포인터
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
			함수명	: execptionFilter
			설명		: 유저 미니덤프 정보를 쓰기 위한 예외 필터 함수
			리턴값	: LONG WINAPI
			매개변수	: _EXCEPTION_POINTERS*
			*****************************************************************************/
			static LONG WINAPI execptionFilter(struct _EXCEPTION_POINTERS *exceptionInfo);
		};
	}
}