#include "stdafx.h"
#include "IOCPServerSession.h"

IOCPServerSession::IOCPServerSession()
	:Session()
{
	m_ID = 0;
	m_Type = SESSION_TYPE_SERVER;
	m_arrIOData[IO_READ].SetType(IO_READ);
	m_arrIOData[IO_WRITE].SetType(IO_WRITE);
}

IOCPServerSession::~IOCPServerSession()
{
}

bool IOCPServerSession::PacketParsing(T_PACKET * pakcet)
{
	T_PACKET* packet = pakcet;
	bool IsSuccess = false;
	int errorNum = 0;
	char nickname[15] = {0, };

	switch (packet->type)
	{
	case PK_NONE:
		cout << "test : " << packet->buff << endl;
		break;

	case PK_ANS_LOGIN:
		IsSuccess = false;
		memcpy(&IsSuccess, packet->buff, sizeof(bool));
		errorNum = 0;
		memcpy(&errorNum, packet->buff +
			sizeof(bool), sizeof(errorNum));
		
		memcpy(nickname, packet->buff +
			sizeof(bool) + sizeof(int), sizeof(nickname));
		if (IsSuccess)
		{
			cout << "로그인 성공" << endl;
			cout << "닉네임 : " << nickname << endl;
		}
		else
		{
			cout << "로그인 실패 " << endl;
			cout << "오류 번호 :" << errorNum << endl;
		}
		break;

	case PK_ANS_CREATE_ID:
		IsSuccess = false;
		memcpy(&IsSuccess, packet->buff, sizeof(bool));
		errorNum = 0;
		memcpy(&errorNum, packet->buff +
			sizeof(bool), sizeof(errorNum));

		if (IsSuccess)
		{
			cout << "아이디 생성 성공" << endl;
		}
		else
		{
			cout << "아이디 생성 실패 " << endl;
			cout << "오류 번호 :" << errorNum << endl;
		}
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
		//종료됨
	}

	SOCKADDR_IN serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	inet_pton(AF_INET, serverIp, &serverAddr.sin_addr);
	serverAddr.sin_port = htons(serverPort);

	if (connect(m_Socket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		WSAERROR->err_quit("connect()");
		//종료됨
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
