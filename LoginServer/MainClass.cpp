#include "stdafx.h"
#include "MainClass.h"

namespace LoginServer
{
	MainClass::MainClass()
	{
		Util::StartMemoryLeakCheck();
	}

	MainClass::~MainClass()
	{
	}

	BOOL MainClass::Init()
	{
		is_On = true;

		//HRESULT hVal = MEMORYMANAGER->Init();
		//if (SUCCEEDED(hVal))
		//{
		//	hVal = THREADPOOLMANAGER->Init();
		//}
		//if (SUCCEEDED(hVal))
		//{
		//	hVal = DATABASE->InitDB();
		//}
		//if (SUCCEEDED(hVal))
		//{
		//	hVal = CLIENTSESSIONMANAGER->Init();
		//}
		//
		//if (SUCCEEDED(hVal))
		//{
		//	hVal = SERVERNETWORK->Init();
		//}
		//
		//return hVal;
		return true;
	}
}