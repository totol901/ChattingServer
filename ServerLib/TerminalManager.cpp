#include "stdafx.h"
#include "TerminalManager.h"

namespace ServerEngine
{
	namespace NetworkSystem
	{
		TerminalManager::TerminalManager()
		{
		}

		TerminalManager::~TerminalManager()
		{
		}

		HRESULT TerminalManager::Init(Server *server)
		{
			m_pServer = server;

			tinyxml2::XMLDocument config;
			if (!Util::loadConfig(&config)) 
			{
				return E_FAIL;
			}

			tinyxml2::XMLElement *root = config.FirstChildElement("App")->FirstChildElement("Terminal");
			tinyxml2::XMLHandle terminalNode = tinyxml2::XMLHandle(root);
			if (!root) 
			{
				SLogPrintAtFile(L"@ not exist terminal setting");
				return E_FAIL;
			}

			tinyxml2::XMLElement *elem = terminalNode.FirstChildElement().ToElement();
			while (elem) 
			{
				array<WCHAR, _MAX_PATH> terminalName;
				Util::StrConvA2W((char *)elem->Value(), terminalName.data(), terminalName.max_size());

				Terminal *terminal = new Terminal(m_pServer, terminalName.data());
				terminal->Init(elem);
				this->put(terminalName.data(), terminal);

				elem = elem->NextSiblingElement();
			}

			SLogPrintAtFile(L"### Terminal set ###");

			return S_OK;
		}

		void TerminalManager::Release()
		{
			for (auto itr : m_terminalPool)
			{
				auto terminal = itr.second;
				SAFE_DELETE(terminal);
			}
		}

		void TerminalManager::put(wstring serverName, Terminal * terminal)
		{
			m_terminalPool.insert(make_pair(serverName, terminal));
		}

		Terminal * TerminalManager::FindTerminal(wstring name)
		{
			auto itr = m_terminalPool.find(name);
			if (itr == m_terminalPool.end()) 
			{
				return nullptr;
			}

			return itr->second;
		}

		bool TerminalManager::isTerminal(const char * ip)
		{
			for (auto terminal : m_terminalPool)
			{
				if (!strcmp(terminal.second->GetIP(), ip)) 
				{
					return true;
				}
			}

			return false;
		}
	}
}