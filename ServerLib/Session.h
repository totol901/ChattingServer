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
	SOCKET m_Socket;
	INT64	m_Id;
	int8_t	m_Type;
	
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
	SOCKET & GetSocket() { return m_Socket; }
	//wstr_t				clientAddress();
	INT64 GetId() { return m_Id; }
	int8_t GetType() { return m_Type; }
	//tick_t				heartBeat();

	//set함수
	void SetId(INT64 id) { m_Id = id; }
	void SetType(int8_t type) { m_Type = type; }
};
