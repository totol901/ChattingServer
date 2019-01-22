#include "stdafx.h"
#include "Server.h"

namespace ServerEngine
{
	namespace NetworkSystem
	{
		Server::Server(System::ContentsProcess *contentsProcess)
			:m_pContentsProcess(contentsProcess),
			m_status(SERVER_STOP)
		{
		}

		Server::~Server()
		{
		}

		HRESULT Server::Init()
		{
			tinyxml2::XMLDocument config;

			if (!Util::loadConfig(&config))
			{
				return E_FAIL;
			}

			//서버 설정
			tinyxml2::XMLElement* root = config.FirstChildElement("App")->FirstChildElement("Server");
			if (!root)
			{
				SLogPrintAtFile(L"@ not exist server setting");
				return E_FAIL;
			}
			tinyxml2::XMLElement* elem = root->FirstChildElement("IP");
			strcpy_s(m_ip, elem->GetText());

			elem = root->FirstChildElement("Port");
			sscanf_s(elem->GetText(), "%d", &m_port);

			elem = root->FirstChildElement("ThreadCount");
			sscanf_s(elem->GetText(), "%d", &m_workerThreadCount);

			m_status = SERVER_INITIALZE;
			SLogPrintAtFile(L"* IO worker thread count : %d", m_workerThreadCount);

			root = config.FirstChildElement("App");
			elem = root->FirstChildElement("Name");

			SLogPrintAtFile(L"### %S start!!! ###", elem->GetText());

			return S_OK;
		}

		void Server::Release()
		{
			shutdownServer();

			m_status = SERVER_STOP;
			SAFE_DELETE(m_pContentsProcess);

			SLogPrintAtFile(L"# End network base");
		}

		void Server::putPackage(Package * package)
		{
			m_pContentsProcess->putPackage(package);
		}
	}
}
