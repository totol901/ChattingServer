// ChattingServer.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//
#include "stdafx.h"

bool is_On = false;

int main()
{
	MINIDUMP->Init();

	Util::StartMemoryLowFragmentationHeap();
	//System::BreakOutMemoryAllocCount(899);

	//한글 로컬로 세팅
	_wsetlocale(LC_ALL, L"korean");
	
	MainClass* mainClass = new MainClass;
	
	if (FAILED(mainClass->Init()))
	{
		return -1;
	}

	mainClass->Update();

	mainClass->Release();

	SAFE_DELETE(mainClass);

    return 0;
}
