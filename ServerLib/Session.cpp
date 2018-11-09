#include "stdafx.h"
#include "Session.h"

bool Session::setSocketOpt()
{
	//���� ���� äũ�� ����
	tcp_keepalive keepAliveSet = { 0 }, returned = { 0 };
	keepAliveSet.onoff = 1;					// KEEPALIVE ON
	keepAliveSet.keepalivetime = 3000;		// Keep Alive in 3 sec.
	keepAliveSet.keepaliveinterval = 3000;	// Resend if No-Reply (10ȸ �ݺ�)

	DWORD dwBytes;
	if (WSAIoctl(
		m_Socket,				//�Է� ����
		SIO_KEEPALIVE_VALS,		//������ ����ֳ� äũ�ϴ� ���� �ɼ�
		&keepAliveSet,			//���� äũ ���� (�ι���)
		sizeof(keepAliveSet),	//�ι��� ũ��
		&returned,				//�ƿ� ����
		sizeof(returned),		//�ƿ� ���� ũ��
		&dwBytes,				//IOCP���� ���ϵ� ����Ʈũ��
		NULL, NULL
	) != 0)
	{
		return false;
	}
	return true;
}

Session::Session()
	:m_ID(-1),
	m_Type(SESSION_TYPE_NONE),
	m_Socket(NULL)
{
	m_arrIOData[IO_READ].SetType(IO_READ);
	m_arrIOData[IO_WRITE].SetType(IO_WRITE);
}

Session::~Session()
{
	m_Type = SESSION_TYPE_NONE;
	closesocket(m_Socket);
}
