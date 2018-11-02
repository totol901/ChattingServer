// ChattingServer.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//
#include "stdafx.h"

int main()
{
	//네트워크 초기화
	SERVERNETWORK->Init();
	
	double MoniteringSec = 0.0;
	while (1)
	{
		TIMER->Update();

		//2초마다 서버 모니터링
		MoniteringSec += TIMER->ElipsedSec();
		if (MoniteringSec > 2.0)
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
