/******************************************************************************
���ϸ�	: DBSession.h
����		: ������ ���̽� ����
******************************************************************************/
#pragma once

class DBSession : 
	public ServerEngine::Singleton<DBSession>,
	public DatabaseSystem::Database
{
	friend Singleton;
private:
	DBSession();
	~DBSession();
	DBSession(const DBSession&) = delete;
	DBSession(const DBSession&&) = delete;
	DBSession& operator = (const DBSession&) = delete;
	DBSession& operator = (const DBSession&&) = delete;

public:
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

	
};
