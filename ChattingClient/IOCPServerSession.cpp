#include "stdafx.h"
#include "IOCPServerSession.h"

IOCPServerSession::IOCPServerSession()
	:Session()
{
	m_Id = 0;
	m_Type = SESSION_TYPE_SERVER;
	m_arrIOData[IO_READ].SetType(IO_READ);
	m_arrIOData[IO_WRITE].SetType(IO_WRITE);
}

IOCPServerSession::~IOCPServerSession()
{
	if (m_Socket != NULL)
	{
		closesocket(m_Socket);
	}
}

bool IOCPServerSession::PacketParsing(char * buff)
{
	T_PACKET* packet = nullptr;
	packet = (T_PACKET*)buff;

	switch (packet->type)
	{
	case PK_NONE:
		cout << "test : " << ((string*)(&packet->buff))->c_str() << endl;
		break;

	case PK_ANS_LOGIN:

		break;
	case PK_ANS_CREATE_ID:

		break;

	case PK_ANS_WAITINGCHANNAL_ENTER:
		break;

	case PK_ANS_WAITINGCHANNAL_CHREAT_CHANNAL:
		break;

	case PK_ANS_WAITINGCHANNAL_CHANNAL_JOIN:
		break;

	case PK_RECV_CHANNAL_MESSAGE:
		break;

	case PK_ANS_CHANNAL_OUT:
		break;

	case PK_ANS_EXIT:
		break;
	}

	if (!m_bConnected)
	{
		return false;
	}

	return true;
}

void IOCPServerSession::SendPacket(T_PACKET packet)
{
	if (!m_arrIOData[IO_WRITE].SetData(packet))
	{
		cout << "Send error" << endl;
		return;
	}

	WSABUF wsaBuf;
	wsaBuf.buf = m_arrIOData[IO_WRITE].GetDataBuff();
	wsaBuf.len = SOCKET_BUFF_SIZE;

	this->Send(wsaBuf);
	this->RecvStandBy();
}

void IOCPServerSession::Disconnect()
{
	closesocket(m_Socket);
	m_bConnected = false;
	m_Socket = NULL;
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
	m_arrIOData[IO_READ].SetTotalBytes();

	if (IsRecving(transferSize))
	{
		return nullptr;
	}

	const size_t packetHeaderSize = sizeof(int32_t);
	int32_t packetDataSize = (int32_t)(m_arrIOData[IO_READ].GetTotalByte() - packetHeaderSize);
	char *packetData = m_arrIOData[IO_READ].GetDataBuff();

	T_PACKET* tmp = (T_PACKET*)packetData;

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
