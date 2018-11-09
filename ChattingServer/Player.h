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
	string			m_PlayerID;
	string			m_PlayerNickname;
	Channel*		m_pChannel;

public:
	Player();
	~Player();

	void SetPlayerState(const E_PLAYER_STATE& state) { m_PlayerState = state; }
	void SetChannel(Channel* channel) { m_pChannel = channel; }
	void SetPlayerID(string playerID) { m_PlayerID = playerID; }
	void SetPlayerNickname(string playerNickname) { m_PlayerNickname = playerNickname; }

	const E_PLAYER_STATE&	GetPlayerState()	{ return m_PlayerState; }
	string&					GetPlayerNickname() { return m_PlayerNickname; }
	string&					GetPlayerID()		{ return m_PlayerID; }
	Channel*				GetChannel() const	{return m_pChannel;}
};
