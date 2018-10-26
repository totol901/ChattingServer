// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>



// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
#include <WinSock2.h>
#include <Windows.h>
#include <iostream>
#include <ws2tcpip.h>

using namespace std;

#pragma comment(lib, "ws2_32.lib")

#include "Util.h"
#include "Singleton.h"
#include "WinSock.h"
#include "Error.h"


#define SERVERPORT 9000

#define WSAWINSOCK WinSock::GetInstance()
#define WSAERROR Error::GetInstance()
