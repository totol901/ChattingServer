#include "stdafx.h"
#include "IOCPServerSession.h"

IOCPServerSession::IOCPServerSession()
	:Session()
{
	m_arrIOData[IO_READ].SetType(IO_READ);
	m_arrIOData[IO_WRITE].SetType(IO_WRITE);
}

IOCPServerSession::~IOCPServerSession()
{
	closesocket(m_Socket);
}

void IOCPServerSession::sendPacket(char * packet)
{
	if (!m_arrIOData[IO_WRITE].SetData(packet))
	{
		return;
	}

	WSABUF wsaBuf;
	wsaBuf.buf = m_arrIOData[IO_WRITE].GetDataBuff();
	wsaBuf.len = sizeof(m_arrIOData[IO_WRITE].GetDataBuff());

	this->Send(wsaBuf);
	this->RecvStandBy();
}

void IOCPServerSession::OnConnect(const char* serverIp, u_short serverPort)
{
	m_Socket = WSASocket(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (m_Socket == INVALID_SOCKET)
	{
		WSAERROR->err_quit("socket()");
		//Á¾·áµÊ
	}

	SOCKADDR_IN serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	inet_pton(AF_INET, serverIp, &serverAddr.sin_addr);
	serverAddr.sin_port = htons(serverPort);

	if (connect(m_Socket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		WSAERROR->err_quit("connect()");
		//Á¾·áµÊ
	}

	m_bConnected = true;
}

void IOCPServerSession::Recv(WSABUF wsabuf)
{
	DWORD flags = 0;
	DWORD recvBytes = 0;
	DWORD errorCode = WSARecv(m_Socket, &wsabuf, 1, &recvBytes, 
		&flags, m_arrIOData[IO_READ].GetOverlapped(), NULL);
}

bool IOCPServerSession::IsRecving(size_t transferSize)
{
	if (m_arrIOData[IO_READ].NeedMoreIO(transferSize))
	{
		Recv(m_arrIOData[IO_READ].GetCurrentWSABuf());
		return true;
	}
	return false;
}

void IOCPServerSession::Send(WSABUF wsaBuf)
{
	DWORD flags = 0;
	DWORD sendBytes;
	DWORD errorCode = WSASend(m_Socket,
		&wsaBuf, 1, &sendBytes, flags,
		m_arrIOData[IO_WRITE].GetOverlapped(), NULL);
}

void IOCPServerSession::OnSend(size_t transferSize)
{
	if (m_arrIOData[IO_WRITE].NeedMoreIO(transferSize))
	{
		Send(m_arrIOData[IO_WRITE].GetCurrentWSABuf());
	}
}

string* IOCPServerSession::OnRecv(size_t transferSize)
{
	int32_t offset = 0;
	offset += m_arrIOData[IO_READ].SetTotalBytes();

	if (IsRecving(transferSize))
	{
		return nullptr;
	}

	const size_t packetHeaderSize = sizeof(int32_t);
	int32_t packetDataSize = (int32_t)(m_arrIOData[IO_READ].GetTotalByte() - packetHeaderSize);
	char *packetData = m_arrIOData[IO_READ].GetDataBuff() + offset;

	string* packet = new string(packetData);
	
	RecvStandBy();
	
	return packet;
}

void IOCPServerSession::RecvStandBy()
{
	m_arrIOData[IO_READ].Clear();

	WSABUF wsaBuf;
	wsaBuf.buf = m_arrIOData[IO_READ].GetDataBuff();
	wsaBuf.len = SOCKET_BUFF_SIZE;

	Recv(wsaBuf);
}
