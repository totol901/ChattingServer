#include "stdafx.h"
#include "ClientSession.h"


ClientSession::ClientSession()
	:Session()
{
	m_arrIOData[IO_READ].SetType(IO_READ);
	m_arrIOData[IO_WRITE].SetType(IO_WRITE);
}


ClientSession::~ClientSession()
{
}
