#pragma once

class ClientSessionManager : public Singleton<ClientSessionManager>
{
private:
	UINT m_GenerateSessionID;

public:
	ClientSessionManager();
	~ClientSessionManager();

	UINT GetGenerateSessionID();
};

