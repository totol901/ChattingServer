// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//
#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
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

#pragma comment(lib, "ws2_32.lib")

using namespace std;

#define PACKET_MAX_SIZE 128
#define PAKCET_BUFF_SIZE (PACKET_MAX_SIZE - sizeof(DWORD) - sizeof(int))

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
