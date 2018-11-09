/******************************************************************************
파일명	: Session.h
목적		: 세션, 부모 클래스
******************************************************************************/
#pragma once

enum
{
	SESSION_TYPE_NONE,
	SESSION_TYPE_TERMINAL,
	SESSION_TYPE_CLIENT,
	SESSION_TYPE_SERVER,
};

class Session
{
protected:
	IOData		m_arrIOData[2];
	SOCKET		m_Socket;
	INT64		m_ID;
	int8_t		m_Type;
	SOCKADDR_IN m_ClientAddr;
	
	

public:
	Session();
	virtual ~Session();

	//TODO : 실제 게임 만들때 필요하니 나중에 구현할 것
	//void				updateHeartBeat();

	//소켓 옵션(좀비 소켓 체크) 설정
	bool setSocketOpt();

public:
	//get함수
	const SOCKET&		GetSocket() const { return m_Socket; }
	const INT64&		GetId() const { return m_ID; }
	const int8_t&		GetType() const { return m_Type; }
	const SOCKADDR_IN&	GetClientAddr() const { return m_ClientAddr; }

	//set함수
	void SetSocketAddr(const SOCKADDR_IN& addr) { m_ClientAddr = addr ;}
	void SetSocket(const SOCKET& socket) { m_Socket = socket; }
	void SetId(const INT64& id) { m_ID = id; }
	void SetType(const int8_t& type) { m_Type = type; }
};
