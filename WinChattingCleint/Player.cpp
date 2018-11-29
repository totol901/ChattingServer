#include "stdafx.h"
#include "Player.h"

Player::Player(const TCHAR* nodename)
	:GameNode(nodename),
	m_PlayerState(PLAYER_NONE)
{
}

Player::~Player()
{
}
