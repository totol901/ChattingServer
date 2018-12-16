/******************************************************************************
파일명	: Player.h
목적		: 클라이언트의 플레이어를 추상화
******************************************************************************/
#pragma once

enum E_PLAYER_STATE
{
	PLAYER_NONE,
	PLAYER_LOGIN,
	PLAYER_WAITING_CHANNEL,
	PLAYER_IN_CHANNEL,
	PLAYER_LOGOUT,
};
class Player : public GameObject
{
private:
	E_PLAYER_STATE	m_PlayerState;
	wstring			m_PlayerID;
	wstring			m_PlayerNickname;
	Channel*		m_pChannel;

	bool IsMove;

	float m_DirectionX;
	float m_DirectionY;
	float m_Velocity;

	

public:
	Player();
	~Player();
	
	chrono::high_resolution_clock::time_point m_DoStartTimePoint;

	void Move();
	bool IsInMovePos(float x, float y);

	const bool& GetIsMove() const
	{
		return IsMove;
	}
	void SetIsMove(const bool& ismove)
	{
		IsMove = ismove;
	}

	void SetVelocity(float Velocity) { m_Velocity = Velocity; }
	
	void SetDirectionX(float x) { m_DirectionX = x; }
	void SetDirectionY(float y) { m_DirectionY = y; }
	
	void SetDirection(float x, float y) { m_DirectionX = x; m_DirectionY = y; }
	void SetPlayerState(const E_PLAYER_STATE& state) { m_PlayerState = state; }
	void SetChannel(Channel* channel) { m_pChannel = channel; }
	void SetPlayerID(wstring playerID) { m_PlayerID = playerID; }
	void SetPlayerNickname(wstring playerNickname) { m_PlayerNickname = playerNickname; }

	float& GetVelocity() { return m_Velocity; }
	
	float& GetDirectionX() { return m_DirectionX; }
	float& GetDirectionY() { return m_DirectionY; }

	const E_PLAYER_STATE&	GetPlayerState()	{ return m_PlayerState; }
	wstring&				GetPlayerNickname() { return m_PlayerNickname; }
	wstring&				GetPlayerID()		{ return m_PlayerID; }
	Channel*				GetChannel() const	{return m_pChannel;}
};
