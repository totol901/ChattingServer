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
	IOData m_arrIOData[2];
	SOCKET m_Socket;
	INT64	m_ID;
	int8_t	m_Type;
	SOCKADDR_IN m_ClientAddr;
	
	//bool setSocketOpt();

public:
	Session();
	virtual ~Session();

	/****************************************************************************
	함수명	: updateHeartBeat
	설명		: lastHeartBeat_을 갱신함
	리턴값	: 없음
	매개변수	: 없음
	*****************************************************************************/
	//void				updateHeartBeat();

public:
	//get함수
	const SOCKET& GetSocket() const { return m_Socket; }
	//wstr_t				clientAddress();
	const INT64& GetId() const { return m_ID; }
	const int8_t& GetType() const { return m_Type; }
	const SOCKADDR_IN& GetClientAddr() const { return m_ClientAddr; }
	//tick_t				heartBeat();

	//set함수
	void SetSocketAddr(const SOCKADDR_IN& addr) { m_ClientAddr = addr ;}
	void SetSocket(const SOCKET& socket) { m_Socket = socket; }
	void SetId(const INT64& id) { m_ID = id; }
	void SetType(const int8_t& type) { m_Type = type; }
};
