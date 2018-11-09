#include "stdafx.h"
#include "MainClass.h"

MainClass::MainClass()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//네트워크 초기화
	SERVERNETWORK->Init();
	//데이터베이스 초기화
	DATABASE->InitDB();
}

MainClass::~MainClass()
{
}

void MainClass::Update()
{
	double MoniteringSec = 0.0;
	while (1)
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
}