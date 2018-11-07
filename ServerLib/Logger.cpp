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
	if (fopen_s(&pFile, (TIMER->Today() + ".log").c_str(), "a+") != 0)
	{
		printf("Log 颇老 积己 角菩\n");
		return;
	}

	printf("Log 颇老 积己 己傍\n");
}

void Logger::LogPrint(const char* str, ...)
{
	string tempStr = TIMER->NowTimeWithMilliSec();
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

void Logger::LogPrintAtFile(const char * str, ...)
{
	string tempStr = TIMER->NowTimeWithMilliSec();
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
}
