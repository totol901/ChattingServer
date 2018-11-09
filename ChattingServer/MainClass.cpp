#include "stdafx.h"
#include "MainClass.h"

MainClass::MainClass()
	:m_isOn(true)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//��Ʈ��ũ �ʱ�ȭ
	SERVERNETWORK->Init();
	//�����ͺ��̽� �ʱ�ȭ
	DATABASE->InitDB();

	//����͸� �� ������ ����
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

	//���� ��ɾ� �Է� ����
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

	return 0;
}
