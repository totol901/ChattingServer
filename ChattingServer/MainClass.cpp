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

	//서버 명령어 입력 받음
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
	SLogPrintAtFile("CPU : %0.2f%%, 사용 메모리 : %u byte, 동접자 : %u",
		MONITORING->processCpuUsage(),
		MONITORING->processMemUsage(),
		CLIENTSESSIONMANAGER->GetCCU()
	);
	
	//메모리 풀 사용량
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
//	OutputDebugStringA("모니터링 스레드 종료");
//
//	return 0;
//}
