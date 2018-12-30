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
			//���� ���� ���� ���Ѱ��� execptionFilter�Լ��� ���� ���� �߻� ó�� ����
			::SetUnhandledExceptionFilter(execptionFilter);
			printf("* Dump filter setting complte!\n");

			return S_OK;
		}

		void MiniDump::Release()
		{
		}

		LONG __stdcall MiniDump::execptionFilter(_EXCEPTION_POINTERS * exceptionInfo)
		{
			//������ ������ �� ���� �Ǵ� ���α׷� ������ ���� �κп��� ��ü ������ ������
			_CrtMemDumpAllObjectsSince(NULL);

			//DBGHELP.DLL �ҷ���
			HMODULE dumpDll = nullptr;
			dumpDll = ::LoadLibraryA("DBGHELP.DLL");
			if (!dumpDll)
			{
				printf("! DBGHelp.dll not loaded\n");
				return 0;
			}

			//dumpPatch�� ��¥.dmp �־���
			wstring dumpPatch;
			dumpPatch += TIMER->NowTimeWithMilliSec();
			dumpPatch += L".dmp";

			//dumpPatch��ġ�� ���� ����
			HANDLE file = ::CreateFile(dumpPatch.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			if (file == INVALID_HANDLE_VALUE)
			{
				printf("! dump file not making\n");
				return 0;
			}

			//�Ͼ ���� �����ϴ� ����ü
			_MINIDUMP_EXCEPTION_INFORMATION info;
			info.ThreadId = ::GetCurrentThreadId();
			info.ExceptionPointers = exceptionInfo;
			info.ClientPointers = NULL;

			//dumpDll�� MiniDumpWriteDump�Լ� �ҷ���
			WRITEDUMP dumpFunc = (WRITEDUMP)::GetProcAddress(dumpDll, "MiniDumpWriteDump");
			//file�� ������� �̴ϴ��� ������ ��
			if (dumpFunc(
				GetCurrentProcess(),		//������ ������ ���μ��� �ڵ�
				GetCurrentProcessId(),		//���μ��� �ĺ���
				file,						//�� ���� �ڵ�
				MiniDumpNormal,				//�������� ���� ���� �����ϵ��� ������ ������
				&info,						//�Ͼ ���� �����ϴ� ����ü
				NULL,						//���� ���� ���� ��� ����
				NULL						//�ݺ� ��� ����
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
