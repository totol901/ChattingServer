#include "stdafx.h"
#include "ClientSession.h"
#include "Player.h"

UINT ClientSession::m_AllocatorID = 0;

ClientSession::ClientSession()
	:Session(),
	m_SessionParser(this)
{
	m_Type = SESSION_TYPE_CLIENT;
	//m_pSessionParser = new ClientSessionParser(this);
	//m_pPlayerData = new Player();
}

ClientSession::~ClientSession()
{
	Clear();
	//SAFE_DELETE(m_pPlayerData);
	//SAFE_DELETE(m_pSessionParser);
	closesocket(m_Socket);
}

void ClientSession::Clear()
{
	//채널에 있을수 있으니 있다면 빠져나옴
	if (m_PlayerData.GetChannel())
	{
		m_PlayerData.GetChannel()->DeleteClientSession(this);
		m_PlayerData.GetChannel()->ChannelErase();
	}
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
	if (m_arrIOData[IO_READ].NeedMoreIO(transferSize))
	{
		Recv(m_arrIOData[IO_READ].GetCurrentWSABuf());
		return true;
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

 const bool ClientSession::PacketParsing(T_PACKET * const packet)
 {
	 if (m_SessionParser.PacketParsing(packet))
	 {
		 return true;
	 }
	
	 return false;
 }

 void ClientSession::SendPacket(const T_PACKET & packet)
 {
	 if (!m_arrIOData[IO_WRITE].SetData(packet))
	 {
		 SLogPrintAtFile("Send error");
		 return;
	 }

	 WSABUF wsaBuf;
	 wsaBuf.buf = (char*)m_arrIOData[IO_WRITE].GetptPacket();
	 wsaBuf.len = packet.Size;

	 this->Send(wsaBuf);
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

	 //패킷 하나를 다 받은 뒤에 다시 recv 해준다
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
