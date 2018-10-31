#include "stdafx.h"
#include "ClientSession.h"

ClientSession::ClientSession()
	:Session(),
	m_bConnected(true)
{
	m_Type = SESSION_TYPE_CLIENT;
	m_arrIOData[IO_READ].SetType(IO_READ);
	m_arrIOData[IO_WRITE].SetType(IO_WRITE);
}

ClientSession::~ClientSession()
{
}

void ClientSession::Send(const WSABUF & wsaBuf)
{
	DWORD flags = 0;
	DWORD sendBytes;
	DWORD errorCode = WSASend(m_Socket,
		(LPWSABUF)&wsaBuf, 1, &sendBytes, flags,
		m_arrIOData[IO_WRITE].GetptOverlapped(), NULL);
}

void ClientSession::Recv(const WSABUF & wsabuf)
{
	DWORD flags = 0;
	DWORD recvBytes = 0;
	DWORD errorCode = WSARecv(m_Socket, (LPWSABUF)&wsabuf, 
		1, &recvBytes, &flags, 
		m_arrIOData[IO_READ].GetptOverlapped(), NULL);
}

bool ClientSession::IsRecving(const size_t& transferSize)
{
	//if (m_arrIOData[IO_READ].NeedMoreIO(transferSize))
	{
		Recv(m_arrIOData[IO_READ].GetCurrentWSABuf());
		//return true;
	}
	return false;
}

 IOData * const ClientSession::GetptIOData(const int & type)
{
	switch (type)
	{
	case IO_READ:
		return &m_arrIOData[IO_READ];

	case IO_WRITE:
		return &m_arrIOData[IO_WRITE];

	default:
		return nullptr;
	}
}

 const bool ClientSession::PacketParsing(T_PACKET * const pakcet)
 {
	 T_PACKET* packet = pakcet;

	 switch (packet->type)
	 {
	 case PK_NONE:
		 cout << "test : " << packet->buff << endl;
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

 void ClientSession::SendPacket(const T_PACKET & packet)
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

 T_PACKET * ClientSession::OnRecv(const size_t & transferSize)
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

 void ClientSession::OnSend(size_t transferSize)
 {
	 if (m_arrIOData[IO_WRITE].NeedMoreIO(transferSize))
	 {
		 Send(m_arrIOData[IO_WRITE].GetCurrentWSABuf());
	 }
 }

 void ClientSession::RecvStandBy()
 {
	 m_arrIOData[IO_READ].Clear();

	 WSABUF wsaBuf;
	 wsaBuf.buf = (char*)m_arrIOData[IO_READ].GetptPacket();
	 wsaBuf.len = PACKET_MAX_SIZE;

	 Recv(wsaBuf);
 }
