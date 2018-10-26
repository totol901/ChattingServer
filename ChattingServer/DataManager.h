#pragma once

struct UserData
{
	UINT num_id;
	char id[15];
	char password[15];
	char nickname[15];
};

class DataManager : public Singleton<DataManager>
{
private:
	map<string, UserData*> m_UserDataTable;

public:
	DataManager();
	~DataManager();

	/****************************************************************************
	�Լ���	: insert
	����		: data�� map�� �־���
	*****************************************************************************/
	bool insert(UserData* data);

	/****************************************************************************
	�Լ���	: insert
	����		: id�� map���� ã�� userdata�� ����
	*****************************************************************************/
	UserData* find(const string& id);
};
