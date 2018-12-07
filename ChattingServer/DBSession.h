/******************************************************************************
���ϸ�	: DBSession.h
����		: ������ ���̽� ����
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
	�Լ���	: InitDB
	����		: Mysql �ʱ�ȭ
	*****************************************************************************/
	HRESULT InitDB();
	void Release();

	/****************************************************************************
	�Լ���	: CheckUserInfoQuery
	����		: ID, PW ���� ������ �´��� �Ǵ�
	*****************************************************************************/
	bool CheckUserInfoQuery(const wstring& ID, const wstring& PW);
	bool CheckUserInfoQuery(string ID, string PW);

	/****************************************************************************
	�Լ���	: InsertUserInfoQuery
	����		: ID, PW, nickname ���� ���� ������ DB ���� �� �ߺ� ���� �Ǵ�
	*****************************************************************************/
	bool InsertUserInfoQuery(wstring ID, wstring PW, wstring nickname);
	
	/****************************************************************************
	�Լ���	: DeleteUserInfoQuery
	����		: ID ���� ������ DB ������
	*****************************************************************************/
	bool DeleteUserInfoQuery(wstring ID);

	/****************************************************************************
	�Լ���	: FindNickname
	����		: ID ���� ������ �г��� �ҷ���
	*****************************************************************************/
	wstring FindNickname(wstring ID);

	/****************************************************************************
	�Լ���	: InsertUserLogQuery
	����		: ID�� Log�� DB�� ����
	*****************************************************************************/
	bool InsertUserLogQuery(wstring ID, wstring log);

	/****************************************************************************
	�Լ���	: MySQLError
	����		: ���� ���� ������ ������
	*****************************************************************************/
	const char* MySQLError() const { return mysql_error(conn); }
};
