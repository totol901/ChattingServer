#include "stdafx.h"
#include "Player.h"

Player::Player()
	:GameObject(),
	m_PlayerState(PLAYER_NONE),
	m_pChannel(nullptr)
{
}

Player::~Player()
{
}