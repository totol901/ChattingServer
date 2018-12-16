#include "stdafx.h"
#include "Session.h"

namespace ServerEngine
{
	namespace NetworkSystem
	{
		bool Session::setSocketOpt()
		{
			//좀비 소켓 채크를 위함
			tcp_keepalive keepAliveSet = { 0 }, returned = { 0 };
			keepAliveSet.onoff = 1;					// KEEPALIVE ON
			keepAliveSet.keepalivetime = 3000;		// Keep Alive in 3 sec.
			keepAliveSet.keepaliveinterval = 1000;	// Resend if No-Reply (10회 반복)

			DWORD dwBytes;
			if (WSAIoctl(
				m_Socket,				//입력 소켓
				SIO_KEEPALIVE_VALS,		//소켓이 살아있나 채크하는 소켓 옵션
				&keepAliveSet,			//소켓 채크 세팅 (인버퍼)
				sizeof(keepAliveSet),	//인버퍼 크기
				&returned,				//아웃 버퍼
				sizeof(returned),		//아웃 버퍼 크기
				&dwBytes,				//IOCP에서 리턴된 바이트크기
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
			InitializeCriticalSection(&m_HartBeatCS);
			InitializeCriticalSection(&m_HeartBeatSendRecvCS);
			m_BeforeBeatTime = TIMER->GetNowTimePoint();
			m_CurrentBeatTime = m_BeforeBeatTime;
			m_SendTime = TIMER->GetNowTime_t();
			m_RecvTime = m_SendTime;

			m_arrIOData[IO_READ].SetType(IO_READ);
			m_arrIOData[IO_WRITE].SetType(IO_WRITE);
		}

		Session::~Session()
		{
			DeleteCriticalSection(&m_HartBeatCS);
			DeleteCriticalSection(&m_HeartBeatSendRecvCS);
			m_Type = SESSION_TYPE_NONE;
			closesocket(m_Socket);
		}

		void Session::Send(const WSABUF & wsaBuf)
		{
			DWORD flags = 0;
			DWORD sendBytes;
			DWORD errorCode = WSASend(m_Socket,
				(LPWSABUF)&wsaBuf, 1, &sendBytes, flags,
				m_arrIOData[IO_WRITE].GetptOverlapped(), NULL);
		}

		void Session::Recv(const WSABUF & wsabuf)
		{
			DWORD flags = 0;
			DWORD recvBytes = 0;
			DWORD errorCode = WSARecv(m_Socket, (LPWSABUF)&wsabuf,
				1, &recvBytes, &flags,
				m_arrIOData[IO_READ].GetptOverlapped(), NULL);
		}

		bool Session::IsRecving(const size_t& transferSize)
		{
			if (m_arrIOData[IO_READ].NeedMoreIO(transferSize))
			{
				Recv(m_arrIOData[IO_READ].GetCurrentWSABuf());
				return true;
			}
			return false;
		}

		void Session::SendPacket(const T_PACKET & packet)
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

		T_PACKET * Session::OnRecv(const size_t & transferSize)
		{
			int32_t offset = 0;
			m_arrIOData[IO_READ].SetTotalBytes();

			if (IsRecving(transferSize))
			{
				return nullptr;
			}

			T_PACKET* packetData = new T_PACKET();
			*packetData = *m_arrIOData[IO_READ].GetptPacket();

			//하트비트 시간 업데이트 해줌
			UpdateHeartBeat();

			//패킷 하나를 다 받은 뒤에 다시 recv 해준다
			RecvStandBy();
			return packetData;
		}

		void Session::OnSend(size_t transferSize)
		{
			if (m_arrIOData[IO_WRITE].NeedMoreIO(transferSize))
			{
				Send(m_arrIOData[IO_WRITE].GetCurrentWSABuf());
			}
		}

		void Session::RecvStandBy()
		{
			m_arrIOData[IO_READ].Clear();

			WSABUF wsaBuf;
			wsaBuf.buf = (char*)m_arrIOData[IO_READ].GetptPacket();
			wsaBuf.len = PACKET_MAX_SIZE;

			Recv(wsaBuf);
		}

		void Session::UpdateHeartBeat()
		{
			//살아있으니 새로 갱신
			EnterCriticalSection(&m_HartBeatCS);
			m_BeforeBeatTime = TIMER->GetNowTimePoint();
			//m_CurrentBeatTime = TIMER->GetNowTimePoint();
			LeaveCriticalSection(&m_HartBeatCS);
		}

		void Session::SendHeartBeat()
		{
			T_PACKET packet;
			const UINT pakcetSize = sizeof(packet.Size) + sizeof(packet.type);

			packet.type = PK_SEND_HEARTBEAT;
			packet.Size = pakcetSize;

			EnterCriticalSection(&m_HeartBeatSendRecvCS);
			m_SendTime = TIMER->GetNowTime_t();
			LeaveCriticalSection(&m_HeartBeatSendRecvCS);

			//SendPacket(packet);
		}

		void Session::RecvHeartBeat()
		{
			EnterCriticalSection(&m_HeartBeatSendRecvCS);
			m_RecvTime = TIMER->GetNowTime_t();
			m_Ping = m_RecvTime - m_SendTime;
			LeaveCriticalSection(&m_HeartBeatSendRecvCS);
		}

		bool Session::ChackHeartBeat()
		{
			//m_CurrentBeatTime = TIMER->GetNowTimePoint();

			//DeadMinTime이 지났다면 세션을 죽었다 판단함
			//EnterCriticalSection(&m_HartBeatCS);
			//chrono::minutes min = TIMER->DuraionMin(m_CurrentBeatTime, m_BeforeBeatTime);
			//LeaveCriticalSection(&m_HartBeatCS);

			//if (DeadMinTime <= min.count())
			//{
			//	//죽음
			//	return false;
			//}
			return true;
		}
	}
}
