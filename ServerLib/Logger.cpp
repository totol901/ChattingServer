#include "stdafx.h"
#include "Logger.h"

Logger::Logger()
	:pFile(nullptr)
{
	Init();
}

Logger::~Logger()
{
	fclose(pFile);
}

void Logger::Init()
{
	char str[1024] = { 0, };
	StrConvW2A((WCHAR*)TIMER->Today().c_str(), str, sizeof(str));
	string str_today = str;
	if (fopen_s(&pFile, (str_today + ".log").c_str(), "a+") != 0)
	{
		printf("Log 颇老 积己 角菩\n");
		return;
	}

	printf("Log 颇老 积己 己傍\n");
}

void Logger::LogPrint(const char* str, ...)
{
	char str1[1024] = { 0, };
	StrConvW2A((WCHAR*)TIMER->NowTimeWithMilliSec().c_str(), str1, sizeof(str1));
	string tempStr = str1;
	tempStr += "... ";

	char buf[1024] = {0,};

	va_list args;
	va_start(args, str);
	vsprintf_s(buf, str, args);
	va_end(args);

	tempStr += buf;
	tempStr += "\n";
	printf(tempStr.c_str());
}


void Logger::LogPrint(const wchar_t* str, ...)
{
	wstring tempStr = TIMER->NowTimeWithMilliSec().c_str();
	tempStr += L"... ";

	wchar_t buf[1024] = { 0, };

	va_list args;
	va_start(args, str);
	vswprintf_s(buf, str, args);
	va_end(args);

	tempStr += buf;
	tempStr += L"\n";
	//wcout << tempStr.c_str() ;
	wprintf_s(tempStr.c_str());
}

void Logger::LogPrintAtFile(const char * str, ...)
{
	char str1[1024] = { 0, };
	StrConvW2A((WCHAR*)TIMER->NowTimeWithMilliSec().c_str(), str1, sizeof(str1));
	string tempStr = str1;
	tempStr += "... ";

	char buf[1024] = { 0, };

	va_list args;
	va_start(args, str);
	vsprintf_s(buf, str, args);
	va_end(args);

	tempStr += buf;
	tempStr += "\n";
	printf(tempStr.c_str());
	fprintf(pFile, tempStr.c_str());
	fflush(pFile);
}

void Logger::LogPrintAtFile(const wchar_t * str, ...)
{
	wstring tempStr = TIMER->NowTimeWithMilliSec().c_str();
	tempStr += L"... ";

	wchar_t buf[1024] = { 0, };
	
	va_list args;
	va_start(args, str);
	vswprintf_s(buf, str, args);
	va_end(args);

	tempStr += buf;
	tempStr += L"\n";
	wprintf(tempStr.c_str());
	fwprintf(pFile, tempStr.c_str());
	fflush(pFile);
}
