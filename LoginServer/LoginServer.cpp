#include "stdafx.h"

bool is_On = false;

int main()
{
	MINIDUMP->Init();

	Util::StartMemoryLowFragmentationHeap();

	//�ѱ� ���÷� ����
	_wsetlocale(LC_ALL, L"korean");



	MINIDUMP->Release();

	return 0;
}