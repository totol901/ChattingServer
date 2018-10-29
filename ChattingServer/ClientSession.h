#pragma once

class ClientSession : public Session
{
private:
	IOData m_arrIOData[2];

public:
	ClientSession();
	~ClientSession();
};

