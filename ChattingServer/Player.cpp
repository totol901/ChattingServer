#include "stdafx.h"
#include "Player.h"

Player::Player()
	:GameObject(),
	m_PlayerState(PLAYER_NONE),
	m_pChannel(nullptr),
	m_PlayerID(L""),
	m_Velocity(0.0f),
	m_DirectionX(0.0f),
	m_DirectionY(0.0f),
	m_DoStartTimePoint(TIMER->GetNowHighTimePoint())
{
}

Player::~Player()
{
}

void Player::Move()
{
	m_LocationX += m_DirectionX * m_Velocity * 0.05f;
	m_LocationY += m_DirectionY * m_Velocity * 0.05f;
}

bool Player::IsInMovePos(float x, float y)
{
	float a = fabsf(m_LocationX - x);
	float b = fabsf(m_LocationY - y);

	if (a + b > 3.0f)
	{
		return false;
	}

	return false;
}
