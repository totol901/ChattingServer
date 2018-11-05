#include "stdafx.h"
#include "IOCPServerSession.h"

IOCPServerSession::IOCPServerSession()
	:Session()
{
	m_ID = 0;
	m_Type = SESSION_TYPE_SERVER;
	m_arrIOData[IO_READ].SetType(IO_READ);
	m_arrIOData[IO_WRITE].SetType(IO_WRITE);

	m_pServerSessionParser = new ServerSessionParser(this);
}

IOCPServerSession::~IOCPServerSession()
{
	SAFE_DELETE(m_pServerSessionParser);
}

bool IOCPServerSession::PacketParsing(T_PACKET * pakcet)
{
	if (m_pServerSessionParser->PacketParsing(pakcet))
	{
		return true;
	}
	return false;
}

void IOCPServerSession::SendPacket(T_PACKET packet)
{
	if (!m_arrIOData[IO_WRITE].SetData(packet))
	{
		cout << "Send error" << endl;
		return;
	}

	WSABUF wsaBuf;
	wsaBuf.buf = (char*)m_arrIOData[IO_WRITE].GetptPacket();
	wsaBuf.len = sizeof(T_PACKET);

	this->Send(wsaBuf);
	//this->RecvStandBy();
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
		&flags, m_arrIOData[IO_READ].GetptOverlapped(), NULL);
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

void IOCPServerSession::AnsLogin(T_PACKET * packet)
{

}

void IOCPServerSession::Send(WSABUF wsaBuf)
{
	DWORD flags = 0;
	DWORD sendBytes;
	DWORD errorCode = WSASend(m_Socket,
		&wsaBuf, 1, &sendBytes, flags,
		m_arrIOData[IO_WRITE].GetptOverlapped(), NULL);
}

void IOCPServerSession::OnSend(size_t transferSize)
{
	if (m_arrIOData[IO_WRITE].NeedMoreIO(transferSize))
	{
		Send(m_arrIOData[IO_WRITE].GetCurrentWSABuf());
	}
}

T_PACKET* IOCPServerSession::OnRecv(size_t transferSize)
{
	int32_t offset = 0;
	m_arrIOData[IO_READ].SetTotalBytes();

	if (IsRecving(transferSize))
	{
		return nullptr;
	}
	
	T_PACKET* packetData = new T_PACKET();
	*packetData = *m_arrIOData[IO_READ].GetptPacket();
	
	RecvStandBy();
	
	return packetData;
}

void IOCPServerSession::RecvStandBy()
{
	m_arrIOData[IO_READ].Clear();

	WSABUF wsaBuf;
	wsaBuf.buf = (char*)m_arrIOData[IO_READ].GetptPacket();
	wsaBuf.len = PACKET_MAX_SIZE;

	Recv(wsaBuf);
}
