#include "stdafx.h"
#include "MiniDump.h"

namespace ServerEngine
{
	namespace Util
	{
		MiniDump::MiniDump()
		{
		}

		MiniDump::~MiniDump()
		{
		}

		HRESULT MiniDump::Init()
		{
			//예외 필터 설정 안한곳에 execptionFilter함수를 통해 예외 발생 처리 설정
			::SetUnhandledExceptionFilter(execptionFilter);
			printf("* Dump filter setting complte!\n");

			return S_OK;
		}

		void MiniDump::Release()
		{
		}

		LONG __stdcall MiniDump::execptionFilter(_EXCEPTION_POINTERS * exceptionInfo)
		{
			//힙에서 지정한 힙 상태 또는 프로그램 실행의 시작 부분에서 개체 정보를 덤프함
			_CrtMemDumpAllObjectsSince(NULL);

			//DBGHELP.DLL 불러옴
			HMODULE dumpDll = nullptr;
			dumpDll = ::LoadLibraryA("DBGHELP.DLL");
			if (!dumpDll)
			{
				printf("! DBGHelp.dll not loaded\n");
				return 0;
			}

			//dumpPatch에 날짜.dmp 넣어줌
			wstring dumpPatch;
			dumpPatch += TIMER->NowTimeWithMilliSec();
			dumpPatch += L".dmp";

			//dumpPatch위치에 파일 생성
			HANDLE file = ::CreateFile(dumpPatch.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			if (file == INVALID_HANDLE_VALUE)
			{
				printf("! dump file not making\n");
				return 0;
			}

			//일어난 예외 설명하는 구조체
			_MINIDUMP_EXCEPTION_INFORMATION info;
			info.ThreadId = ::GetCurrentThreadId();
			info.ExceptionPointers = exceptionInfo;
			info.ClientPointers = NULL;

			//dumpDll의 MiniDumpWriteDump함수 불러옴
			WRITEDUMP dumpFunc = (WRITEDUMP)::GetProcAddress(dumpDll, "MiniDumpWriteDump");
			//file에 유저모드 미니덤프 정보를 씀
			if (dumpFunc(
				GetCurrentProcess(),		//정보가 생성될 프로세스 핸들
				GetCurrentProcessId(),		//프로세스 식별자
				file,						//쓸 파일 핸들
				MiniDumpNormal,				//스레드의 스택 추적 가능하도록 정보를 포함함
				&info,						//일어난 예외 설명하는 구조체
				NULL,						//유저 정의 정보 사용 안함
				NULL						//콜벡 사용 안함
				) == FALSE)
			{
				printf("! dump file saving error\n");
				return 0;
			}
			::CloseHandle(file);

			return EXCEPTION_CONTINUE_SEARCH;
		}
	}
}
