#pragma once

namespace ServerEngine
{
	namespace NetworkSystem
	{
		class TerminalManager : public Singleton<TerminalManager>
		{
			friend Singleton;
		private:
			TerminalManager(const TerminalManager&) = delete;
			TerminalManager(const TerminalManager&&) = delete;
			TerminalManager& operator = (const TerminalManager&) = delete;
			TerminalManager& operator = (const TerminalManager&&) = delete;
			TerminalManager();
			~TerminalManager();

			unordered_map<wstring, Terminal*> m_terminalPool;
			Server* m_pServer;

		public:
			HRESULT Init(Server *server);
			void Release();

			void put(wstring serverName, Terminal *terminal);
			Terminal* FindTerminal(wstring name);

			bool isTerminal(const char *ip);
		};

	}
}
