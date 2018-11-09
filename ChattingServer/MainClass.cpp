#include "stdafx.h"
#include "MainClass.h"

MainClass::MainClass()
	:m_isOn(true)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//네트워크 초기화
	SERVERNETWORK->Init();
	//데이터베이스 초기화
	DATABASE->InitDB();

	//모니터링 용 스래드 생성
	_beginthreadex(NULL, 0,
		MoniteringThread,
		this,
		0, NULL);
}

MainClass::~MainClass()
{
}

void MainClass::Update()
{
	char str[1024] = {0, };

	//서버 명령어 입력 받음
	while (m_isOn)
	{
		cin >> str;

		if (strcmp(str, "exit") == 0)
		{
			m_isOn = false;
			break;
		}
	}
}

unsigned int __stdcall MainClass::MoniteringThread(LPVOID param)
{
	MainClass* mainClass = (MainClass*)param;
	double MoniteringSec = 0.0;
	while (mainClass->IsOn())
	{
		TIMER->Update();

		//2초마다 서버 모니터링
		MoniteringSec += TIMER->ElipsedSec();
		if (MoniteringSec > 5.0)
		{
			MoniteringSec = 0.0;
			SLogPrintAtFile("CPU : %0.2f%%, 사용 메모리 : %u byte, 동접자 : %u",
				MONITORING->processCpuUsage(),
				MONITORING->processMemUsage(),
				CLIENTSESSIONMANAGER->GetCCU());
		}
	}

	return 0;
}
