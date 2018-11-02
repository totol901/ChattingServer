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
	if (fopen_s(&pFile, (TIMER->NowTimeWithMilliSec() + ".log").c_str(), "a+") != 0)
	{
		printf("Log 颇老 积己 角菩\n");
		return;
	}

	printf("Log 颇老 积己 己傍\n");
}

void Logger::LogPrint(const char* str, ...)
{
	string tempStr = TIMER->NowTimeWithMilliSec();
	tempStr += ": ";
	tempStr += str;
	tempStr += "\n";

	char* buf = (char*)tempStr.c_str();

	va_list args;
	va_start(args, buf);
	vprintf(buf, args);
	va_end(args);
}

void Logger::LogPrintAtFile(const char * str, ...)
{
	string tempStr = TIMER->NowTimeWithMilliSec();
	tempStr += ": ";
	tempStr += str;
	tempStr += "\n";

	char* buf = (char*)tempStr.c_str();

	va_list args;
	va_start(args, buf);
	vprintf(buf, args);
	vfprintf_s(pFile, buf, args);
	va_end(args);
}
