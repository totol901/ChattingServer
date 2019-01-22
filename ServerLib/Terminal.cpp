#include "stdafx.h"
#include "Terminal.h"

namespace ServerEngine
{
	namespace NetworkSystem
	{
		Terminal::Terminal(Server *server, wstring name)
			:m_pServer(server),
			m_name(name),
			m_pProcessThread(nullptr),
			m_status(TERMINAL_STOP)
		{
		}

		Terminal::~Terminal()
		{
		}

		HRESULT Terminal::Init(tinyxml2::XMLElement* config)
		{
			tinyxml2::XMLElement* elem;

			elem = config->FirstChildElement("IP");
			strcpy_s(m_ip, elem->GetText());

			elem = config->FirstChildElement("Port");
			sscanf_s(elem->GetText(), "%d", &m_port);

			this->run();

			return S_OK;
		}

		void Terminal::Release()
		{
			m_status = TERMINAL_STOP;
		}

		unsigned int __stdcall Terminal::connectProcess(LPVOID param)
		{
			Terminal* temp = (Terminal*)param;

		CONNECT_START:
			int tryCount = 0;
			while (GLOBALVAL->m_IsShutdown == false)
			{
				if (temp->m_session.ConnectTo(temp->m_ip, temp->m_port))
				{
					break;
				}

				SLogPrintAtFile(L"* try connect [%s] server[%S]:[%d]... [%d]",
					temp->m_name.c_str(),
					temp->m_ip,
					temp->m_port,
					tryCount++);
				Sleep(1000);        // 1초마다 연결 시도
			}
			temp->m_status = TERMINAL_READY;

			// 자신이 터미널 세션임을 알려준다.
			PK_I_NOTIFY_TERMINAL terminalPacket;
			temp->SendPacket(&terminalPacket);

			SLogPrintAtFile(L"* [%s]terminal connect [%S]:[%d] ready",
				temp->m_name.c_str(),
				temp->m_ip, 
				temp->m_port);

			while (GLOBALVAL->m_IsShutdown == false)
			{
				//동기화 recv 실시
				Package *package = temp->m_session.OnRecv(0);

				if (package == nullptr)
				{
					SLogPrintAtFile(L"! termnal [%s] disconnected !", 
						temp->m_name.c_str());
						temp->m_session.OnClose();

					goto CONNECT_START;
				}

				temp->m_pServer->putPackage(package);
			}

			return 0;
		}

		void Terminal::run()
		{
			m_pProcessThread = new System::Thread(this, connectProcess);
		}

		void Terminal::SendPacket(Packet * packet)
		{
			if (m_status == TERMINAL_READY)
			{
				m_session.SendPacket(packet);
			}
		}
	}
}