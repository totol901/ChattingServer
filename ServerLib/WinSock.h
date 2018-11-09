/******************************************************************************
파일명	: WinSock.h
목적		: 윈속 초기화 및 종료
사용방식	: 싱글턴 패턴으로 불러오고 종료시켜줌
******************************************************************************/
#pragma once

class WinSock : public Singleton<WinSock>
{
	friend Singleton;
private:
	WinSock();
	~WinSock();

	WSADATA m_WSAData;

public:
	void			Init() {}
	const WSADATA&	GetWSAData();
};
