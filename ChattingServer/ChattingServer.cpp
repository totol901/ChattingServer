// ChattingServer.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//
#include "stdafx.h"

int main()
{
	//한글 로컬로 세팅
	_wsetlocale(LC_ALL, L"korean");
	
	MainClass* mainClass = new MainClass;

	mainClass->Update();

	SAFE_DELETE(mainClass);

    return 0;
}
