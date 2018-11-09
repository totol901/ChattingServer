#include "stdafx.h"
#include "Session.h"

bool Session::setSocketOpt()
{
	//좀비 소켓 채크를 위함
	tcp_keepalive keepAliveSet = { 0 }, returned = { 0 };
	keepAliveSet.onoff = 1;					// KEEPALIVE ON
	keepAliveSet.keepalivetime = 3000;		// Keep Alive in 3 sec.
	keepAliveSet.keepaliveinterval = 3000;	// Resend if No-Reply (10회 반복)

	DWORD dwBytes;
	if (WSAIoctl(
		m_Socket,				//입력 소켓
		SIO_KEEPALIVE_VALS,		//소켓이 살아있나 채크하는 소켓 옵션
		&keepAliveSet,			//소켓 채크 세팅 (인버퍼)
		sizeof(keepAliveSet),	//인버퍼 크기
		&returned,				//아웃 버퍼
		sizeof(returned),		//아웃 버퍼 크기
		&dwBytes,				//IOCP에서 리턴된 바이트크기
		NULL, NULL
	) != 0)
	{
		return false;
	}
	return true;
}

Session::Session()
	:m_ID(-1),
	m_Type(SESSION_TYPE_NONE),
	m_Socket(NULL)
{
	m_arrIOData[IO_READ].SetType(IO_READ);
	m_arrIOData[IO_WRITE].SetType(IO_WRITE);
}

Session::~Session()
{
	m_Type = SESSION_TYPE_NONE;
	closesocket(m_Socket);
}
