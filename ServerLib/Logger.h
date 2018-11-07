#pragma once

#define SLogPrint(arg, ...)	Logger::GetInstance()->LogPrint(arg, __VA_ARGS__);
#define SLogPrintAtFile(arg, ...)	Logger::GetInstance()->LogPrintAtFile(arg, __VA_ARGS__);

#define SErrLogPrint(arg, ...) Logger::GetInstance()->LogPrint(arg, __VA_ARGS__); ExitProcess(0);
#define SErrPrintAtFile(arg, ...) Logger::GetInstance()->LogPrintAtFile(arg, __VA_ARGS__); ExitProcess(0);

class Logger : public Singleton<Logger>
{
	friend Singleton;
private:
	FILE* pFile;
	Logger();
	~Logger();

	void Init();

public:
	void LogPrint(const char* str, ...);
	void LogPrintAtFile(const char* str, ...);

};
