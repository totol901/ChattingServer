#pragma once

class DBSession : public Singleton<DBSession>
{
	friend Singleton;
private:
	DBSession();
	~DBSession();

public:
	bool InitDB();

	bool CheckQuery(string ID, string PW);
	bool InsertQuery(string ID, string PW, string nickname);
	bool DeleteQuery(string ID);
	string FindNickname(string ID);

private:
	const char* DB_HOST;
	const char* DB_NAME;
	const char* DB_SOCK;
	const char* DB_USER;
	const char* DB_PASS;

	enum
	{
		DB_PORT = 3306,
		DB_OPT = 0
	};

	MYSQL* conn;
	MYSQL_RES* res;
	MYSQL_ROW row;
};

