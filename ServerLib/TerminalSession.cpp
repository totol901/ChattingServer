#include "stdafx.h"
#include "TerminalSession.h"

namespace ServerEngine
{
	namespace NetworkSystem
	{
		TerminalSession::TerminalSession()
		{
		}

		TerminalSession::~TerminalSession()
		{
		}

		bool TerminalSession::ConnectTo(char * ip, int port)
		{
			m_Socket = ::socket(AF_INET, SOCK_STREAM, 0);
			if (m_Socket == INVALID_SOCKET)
			{
				SLogPrintAtFile(L"! terminal socket fail");
				return false;
			}
			memset(&m_ClientAddr, 0, sizeof(m_ClientAddr));
			m_ClientAddr.sin_family = AF_INET;
			m_ClientAddr.sin_port = htons(port);
			inet_pton(AF_INET, ip, &(m_ClientAddr.sin_addr));

			this->setSocketOpt();

			int ret = ::connect(m_Socket,
				(sockaddr *)&m_ClientAddr,
				sizeof(m_ClientAddr));
			if (ret == SOCKET_ERROR)
			{
				SLogPrintAtFile(L"! terminal socket connect fail");
				return false;
			}
			return true;
		}

		Package * TerminalSession::OnRecv(size_t transferSize)
		{
			array<BYTE, PACKET_MAX_SIZE> rowData;

			int ret = ::recv(m_Socket, (char *)rowData.data(), (int)rowData.size(), 0);
			if (ret <= 0)
			{
				return nullptr;
			}

			//패킷 길이 가지고 오기  
			int packetLen[1] = { 0, };

			::memcpy_s((void *)packetLen, sizeof(packetLen), (void *)rowData.data(), sizeof(packetLen));
			
			while (ret < (int)packetLen[0])
			{
				int len = ret;
				ret += ::recv(m_Socket, (char *)rowData.data() + len, (int)rowData.size() - len, 0);
			}

			//패킷 다 받아왔으니 패킷 데이터 디코드
			RecvStream stream;
			stream.set((char*)(rowData.data()), packetLen[0]);

			//서버간 패킷 처리
			Packet *packet = PACKETMAKER->GetPacket(stream.GetType());
			if (packet == nullptr)
			{
				return nullptr;
			}

			packet->Decode(stream);

			Package *package = new Package(this, packet);

			return package;
		}
	}
}