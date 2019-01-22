#pragma once

namespace ServerEngine
{
	namespace NetworkSystem
	{
		enum TERMINAL_STATUS
		{
			TERMINAL_STOP,
			TERMINAL_READY,
		};

		class Terminal
		{
		protected:
			Server* m_pServer;
			wstring m_name;
			TERMINAL_STATUS m_status;

			TerminalSession m_session;
			char m_ip[16];
			int	 m_port;

			System::Thread* m_pProcessThread;

			static unsigned int WINAPI connectProcess(LPVOID param);
			void run();

		public:
			Terminal(Server *server, wstring name);
			virtual ~Terminal();

			HRESULT Init(tinyxml2::XMLElement* config);
			void Release();
			void SendPacket(Packet *packet);

			Server* GetServer() { return m_pServer; }
			const char *GetIP() const {return m_ip;}
			const int& GetPort() const { return m_port; }
			const TERMINAL_STATUS &GetStatus() const { return m_status; }
		};
	}
}
