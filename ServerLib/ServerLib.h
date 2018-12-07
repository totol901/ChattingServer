#pragma once


#include <WinSock2.h>
#include <Windows.h>
#include <iostream>
#include <ws2tcpip.h>

#include <map>
#include <vector>
#include <process.h>
#include <array>
#include <psapi.h>
#include <time.h>
#include <set>
#include <mstcpip.h>
#include <crtdbg.h>
#include <locale.h>
#include <functional>
#include <queue>
#include <stack>

using namespace std;

#pragma comment(lib, "ws2_32.lib")

#define PACKET_MAX_SIZE 1024
#define PAKCET_BUFF_SIZE (PACKET_MAX_SIZE - sizeof(UINT) - sizeof(int))

#include "Stream.h"
#include "RecvStream.h"
#include "SendStream.h"
#include "Packet.h"
#include "Util.h"
#include "Singleton.h"
#include "WinSock.h"
#include "IOData.h"
#include "Session.h"
#include "Monitoring.h"
#include "Timer.h"
#include "Logger.h"
#include "Assert.h"
#include "Allocator.h"
#include "PoolAllocator.h"
#include "LinearAllocator.h"
#include "StackAllocator.h"
#include "FreeListAllocator.h"
#include "c_Hash.h"
#include "MemoryManager.h"
#include "Work.h"
#include "WorkThread.h"
#include "ThreadPoolManager.h"

#define WSAWINSOCK WinSock::GetInstance()
#define MONITORING Monitoring::GetInstance()
#define TIMER Timer::GetInstance()
#define MEMORYMANAGER MemoryManager::GetInstance()
#define THREADPOOLMANAGER ThreadPoolManager::GetInstance()

//파싱 에러 상태
enum E_PARSING_ERROR
{
	ERROR_NONE,
	LOGIN_ERROR_WRONG,
	LOGIN_ERROR_ALREADY_LOGIN,
	LOGIN_ERROR_CREATEID_ID_ALREADY_EXE,
	CREATE_CHANNEL_ALREADY_EXE,
	ENTER_CHANNEL_CANT_FIND,
	LEAVE_CHANNEL_CANT_FIND,
};

