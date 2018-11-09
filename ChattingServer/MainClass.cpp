#include "stdafx.h"
#include "MainClass.h"

MainClass::MainClass()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//��Ʈ��ũ �ʱ�ȭ
	SERVERNETWORK->Init();
	//�����ͺ��̽� �ʱ�ȭ
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

		//2�ʸ��� ���� ����͸�
		MoniteringSec += TIMER->ElipsedSec();
		if (MoniteringSec > 5.0)
		{
			MoniteringSec = 0.0;
			SLogPrintAtFile("CPU : %0.2f%%, ��� �޸� : %u byte, ������ : %u",
				MONITORING->processCpuUsage(),
				MONITORING->processMemUsage(),
				CLIENTSESSIONMANAGER->GetCCU());
		}


	}
}