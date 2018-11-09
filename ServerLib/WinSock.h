/******************************************************************************
���ϸ�	: WinSock.h
����		: ���� �ʱ�ȭ �� ����
�����	: �̱��� �������� �ҷ����� ���������
******************************************************************************/
#pragma once

class WinSock : public Singleton<WinSock>
{
	friend Singleton;
private:
	WinSock();
	~WinSock();

	WSADATA m_WSAData;

public:
	void			Init() {}
	const WSADATA&	GetWSAData();
};
