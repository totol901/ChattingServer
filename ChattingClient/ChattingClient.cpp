// ChattingClient.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//
#include "stdafx.h"

int main()
{
	MainClass* mainClass = new MainClass;
	mainClass->Update();

	SAFE_DELETE(mainClass);

    return 0;
}
