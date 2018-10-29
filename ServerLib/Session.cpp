#include "stdafx.h"
#include "Session.h"

Session::Session()
	:m_Id(-1),
	m_Type(SESSION_TYPE_NONE),
	m_Socket(NULL)
{
}

Session::~Session()
{
}
