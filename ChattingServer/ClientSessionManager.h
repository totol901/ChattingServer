/******************************************************************************
���ϸ�	: ClientSessionManager.h
����		: Ŭ���̾�Ʈ ���ǵ��� ������
******************************************************************************/
#pragma once

class ClientSessionManager : public Singleton<ClientSessionManager>
{
	friend Singleton;
private:
	UINT						m_GenerateSessionID;
	map<SOCKET, ClientSession*> m_mapClientSession;		//���Ʈ�� ����
	map<wstring, ClientSession*> m_mapClientSessionID;	//�α��ε� ����

private:
	ClientSessionManager();
	~ClientSessionManager();

	const UINT GetGenerateSessionID();

public:
	bool			AddClientSession( ClientSession* const clientSession);
	bool			AddClientSessionID(wstring id ,ClientSession* const clientSession);
	bool			DeleteClientSession(const SOCKET& socket);
	bool			DeleteClientSessionID(wstring& id);
	ClientSession*	FindClientSession(SOCKET socket);
	ClientSession*	FindClientSessionID(wstring id);

	const size_t GetCCU() const { return m_mapClientSession.size(); }
};
