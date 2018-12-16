#include "stdafx.h"
#include "LoginChattingClientSession.h"
#include "Player.h"

LoginChattingClientSession::LoginChattingClientSession()
	:ClientSession(),
	m_LoginChattingClientSessionParser(this)
{
}

LoginChattingClientSession::~LoginChattingClientSession()
{
}

HRESULT LoginChattingClientSession::Init()
{
	HRESULT hr = ClientSession::Init();

	return hr;
}

void LoginChattingClientSession::Release()
{
	ClientSession::Release();

	//ä�ο� ������ ������ �ִٸ� ��������
	if (m_PlayerData.GetChannel())
	{
		m_PlayerData.GetChannel()->DeleteClientSession(this);
		m_PlayerData.GetChannel()->ChannelErase();
	}
}
