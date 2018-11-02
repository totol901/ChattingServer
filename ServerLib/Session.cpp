#include "stdafx.h"
#include "Session.h"

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
