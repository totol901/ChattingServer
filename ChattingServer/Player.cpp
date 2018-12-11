#include "stdafx.h"
#include "Player.h"

Player::Player()
	:GameObject(),
	m_PlayerState(PLAYER_NONE),
	m_pChannel(nullptr),
	m_PlayerID(L""),
	m_LocationX(0.0f),
	m_LocationY(0.0f),
	m_Acceleration(1.3f)
{
}

Player::~Player()
{
}
