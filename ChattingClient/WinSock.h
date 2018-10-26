/******************************************************************************
파일명	: WinSock.h
목적		: 윈속 초기화 및 종료
사용방식	: 싱글턴 패턴으로 불러오고 종료시켜줌
******************************************************************************/
#pragma once

class WinSock : public Singleton<WinSock>
{
private:
	WSADATA m_WSAData;

	WinSock();

public:
	~WinSock();

public:
	const WSADATA& GetWSAData();
};
