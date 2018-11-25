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

#pragma comment(lib, "ws2_32.lib")

using namespace std;

#define PACKET_MAX_SIZE 1024
#define PAKCET_BUFF_SIZE (PACKET_MAX_SIZE - sizeof(DWORD) - sizeof(int))

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

#define WSAWINSOCK WinSock::GetInstance()
#define MONITORING Monitoring::GetInstance()
#define TIMER Timer::GetInstance()

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
