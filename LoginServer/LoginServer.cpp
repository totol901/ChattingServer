#include "stdafx.h"

bool is_On = false;

int main()
{
	MINIDUMP->Init();

	Util::StartMemoryLowFragmentationHeap();

	//한글 로컬로 세팅
	_wsetlocale(LC_ALL, L"korean");


	return 0;
}