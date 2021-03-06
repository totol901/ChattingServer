// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
#include "../ServerLib/stdafx.h"
#pragma comment(lib, "ServerLib.lib")
#pragma comment(lib, "libmysql.lib")

#include "MainClass.h"
#include "ClientSession.h"
#include "Object.h"
#include "Channel.h"
#include "ChannelManager.h"
#include "ServerNetwork.h"
#include "ClientSessionManager.h"
#include "mysql.h"
#include "DBSession.h"
#include "GameObject.h"
#include "Player.h"
#include "ClientSessionParser.h"

#define DATABASE DBSession::GetInstance()
#define SERVERNETWORK ServerNetwork::GetInstance()
#define CLIENTSESSIONMANAGER ClientSessionManager::GetInstance()
#define CHANNELMANAGER ChannelManager::GetInstance()
