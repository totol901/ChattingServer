/******************************************************************************
���ϸ�	: WinSock.h
����		: ���� �ʱ�ȭ �� ����
�����	: �̱��� �������� �ҷ����� ���������
******************************************************************************/
#pragma once

class WinSock : public Singleton<WinSock>
{
private:
	WSADATA m_WSAData;

	WinSock();

public:
	~WinSock();

public:
	const WSADATA& GetWSAData();
};
