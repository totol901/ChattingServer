#include "stdafx.h"
#include "MainClass.h"

MainClass::MainClass()
	:m_isOn(true)
{
	System::StartMemoryLeakCheck();
}

MainClass::~MainClass()
{
}

BOOL MainClass::Init()
{
	is_On = true;
	
	HRESULT hVal = MEMORYMANAGER->Init();
	if (SUCCEEDED(hVal))
	{
		hVal = THREADPOOLMANAGER->Init();
	}
	if (SUCCEEDED(hVal))
	{
		hVal = DATABASE->InitDB();
	}
	if (SUCCEEDED(hVal))
	{
		hVal = CLIENTSESSIONMANAGER->Init();
	}

	if (SUCCEEDED(hVal))
	{
		hVal = SERVERNETWORK->Init();
	}

	return hVal;
}

void MainClass::Release()
{
	is_On = false;
	SERVERNETWORK->Release();
	CLIENTSESSIONMANAGER->Release();
	DATABASE->Release();
	THREADPOOLMANAGER->Release();
	MEMORYMANAGER->Release();
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
		if (strcmp(str, "-m") == 0)
		{
			THREADPOOLMANAGER->MakeWork(bind(&MainClass::Monitering, this));
		}
	}
}

void MainClass::Monitering()
{
	SLogPrintAtFile("CPU : %0.2f%%, ��� �޸� : %u byte, ������ : %u",
		MONITORING->processCpuUsage(),
		MONITORING->processMemUsage(),
		CLIENTSESSIONMANAGER->GetCCU()
	);
	
	//�޸� Ǯ ��뷮
	SLogPrintAtFile("WorkClass MemPool : %0.2f%%, ClientSession MemPool : %0.2f%%, PacketClass MemPool : %0.2f%%",
		MEMORYMANAGER->GetAllocator(TEXT("WorkMemory"))->GetUsedMemoryPercentage(),
		MEMORYMANAGER->GetAllocator(TEXT("ClientSessionMemory"))->GetUsedMemoryPercentage()
	);
}

//unsigned int __stdcall MainClass::HartBeatThread(LPVOID param)
//{
//	MainClass* mainClass = (MainClass*)param;
//	double MoniteringSec = 0.0;
//	while (mainClass->IsOn())
//	{
//		
//	}
//
//	OutputDebugStringA("����͸� ������ ����");
//
//	return 0;
//}
