#pragma once

struct UserData
{
	UINT num_id;

};

class DataManager : public Singleton<DataManager>
{
private:

public:
	DataManager();
	~DataManager();
};
