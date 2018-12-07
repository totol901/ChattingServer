/******************************************************************************
파일명	: DBSession.h
목적		: 데이터 베이스 접근
******************************************************************************/
#pragma once

class DBSession : public Singleton<DBSession>
{
	friend Singleton;
private:
	DBSession();
	~DBSession();
	DBSession(const DBSession&) = delete;
	DBSession(const DBSession&&) = delete;
	DBSession& operator = (const DBSession&) = delete;
	DBSession& operator = (const DBSession&&) = delete;

	enum
	{
		DB_PORT = 3306,
		DB_OPT = 0
	};

	const char* DB_HOST;
	const char* DB_NAME;
	const char* DB_SOCK;
	const char* DB_USER;
	const char* DB_PASS;

	MYSQL*		conn;
	MYSQL_RES*	res;
	MYSQL_ROW	row;

public:
	/****************************************************************************
	함수명	: InitDB
	설명		: Mysql 초기화
	*****************************************************************************/
	HRESULT InitDB();
	void Release();

	/****************************************************************************
	함수명	: CheckUserInfoQuery
	설명		: ID, PW 쿼리 보내고 맞는지 판단
	*****************************************************************************/
	bool CheckUserInfoQuery(const wstring& ID, const wstring& PW);
	bool CheckUserInfoQuery(string ID, string PW);

	/****************************************************************************
	함수명	: InsertUserInfoQuery
	설명		: ID, PW, nickname 생성 쿼리 보내고 DB 저장 및 중복 여부 판단
	*****************************************************************************/
	bool InsertUserInfoQuery(wstring ID, wstring PW, wstring nickname);
	
	/****************************************************************************
	함수명	: DeleteUserInfoQuery
	설명		: ID 쿼리 보내서 DB 삭제함
	*****************************************************************************/
	bool DeleteUserInfoQuery(wstring ID);

	/****************************************************************************
	함수명	: FindNickname
	설명		: ID 퀘리 보내서 닉네임 불러옴
	*****************************************************************************/
	wstring FindNickname(wstring ID);

	/****************************************************************************
	함수명	: InsertUserLogQuery
	설명		: ID와 Log를 DB에 저장
	*****************************************************************************/
	bool InsertUserLogQuery(wstring ID, wstring log);

	/****************************************************************************
	함수명	: MySQLError
	설명		: 쿼리 에러 내용을 리턴함
	*****************************************************************************/
	const char* MySQLError() const { return mysql_error(conn); }
};
