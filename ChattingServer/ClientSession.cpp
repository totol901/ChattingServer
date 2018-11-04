#include "stdafx.h"
#include "ClientSession.h"

ClientSession::ClientSession()
	:Session(),
	m_bConnected(true)
{
	m_Type = SESSION_TYPE_CLIENT;
}

ClientSession::~ClientSession()
{
	closesocket(m_Socket);
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

 const bool ClientSession::PacketParsing(T_PACKET * const pakcet)
 {
	 T_PACKET* packet = pakcet;
	 int index = 0;
	 char id[15] = { 0, };
	 char pw[15] = { 0, };
	 char nickname[15] = {0, };
	 bool isSuccess = false;
	 int errorNum = 0;

	 switch (packet->type)
	 {
	 case PK_NONE:
		 cout << "test : " << packet->buff << endl;
		 break;

	 case PK_REQ_LOGIN:
		 index = 0;
		 memcpy(id, pakcet->buff + index, sizeof(id));
		 index += sizeof(id);
		 memcpy(pw, pakcet->buff + index, sizeof(pw));

		 if (DATABASE->CheckQuery(id, pw))
		 {
			 bool isSuccess = true;
			 int errorNum = 0;
			 T_PACKET pk(PK_ANS_LOGIN);
			 int index = 0;
			 memcpy(pk.buff + index,
				 &isSuccess, sizeof(isSuccess));
			 index += sizeof(isSuccess);
			 memcpy(pk.buff + index,
				 &errorNum, sizeof(errorNum));
			 index += sizeof(errorNum);
			 string nickname = DATABASE->FindNickname(id);
			 const char* str = nickname.c_str();
			 memcpy(pk.buff + index,
				 str, nickname.size());

			 SendPacket(pk);
		 }
		 else
		 {
			 bool isSuccess = false;
			 int errorNum = 0;
			 T_PACKET pk(PK_ANS_LOGIN);
			 int index = 0;
			 memcpy(pk.buff + index,
				 &isSuccess, sizeof(isSuccess));
			 index += sizeof(isSuccess);
			 memcpy(pk.buff + index,
				 &errorNum, sizeof(errorNum));

			 SendPacket(pk);
		 }

		 break;

	 case PK_REQ_CREATE_ID:
	 {
		 index = 0;
		 
		 memcpy(id, packet->buff + index, sizeof(id));
		 index += sizeof(id);
		
		 memcpy(pw, packet->buff + index, sizeof(pw));
		 index += sizeof(pw);
		
		 memcpy(nickname, packet->buff + index, sizeof(nickname));
		 
		 if (DATABASE->InsertQuery(id, pw, nickname))
		 {
			 bool isSucces = true;
			 int errNum = 0;
			 T_PACKET pk(PK_ANS_CREATE_ID);
			 int index = 0;
			 memcpy(pk.buff, &isSucces, sizeof(isSucces));
			 index += sizeof(isSucces);
			 memcpy(pk.buff + index, &errNum, sizeof(errNum));

			 SendPacket(pk);
		 }
		 else
		 {
			 bool isSucces = false;
			 int errNum = 0;
			 T_PACKET pk(PK_ANS_CREATE_ID);
			 int index = 0;
			 memcpy(pk.buff, &isSucces, sizeof(isSucces));
			 index += sizeof(isSucces);
			 memcpy(pk.buff + index, &errNum, sizeof(errNum));

			 SendPacket(pk);
		 }
	 }
		 break;

	 case PK_REQ_WAITINGCHANNAL_ENTER:
		 break;

	 case PK_REQ_WAITINGCHANNAL_CHREAT_CHANNAL:
		 break;

	 case PK_REQ_WAITINGCHANNAL_CHANNAL_JOIN:
		 break;

	 case PK_REQ_CHANNAL_SEND_MESSAGE:
		 break;

	 case PK_REQ_CHANNAL_OUT:
		 break;

	 case PK_REQ_EXIT:
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
