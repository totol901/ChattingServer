#pragma once

namespace ServerEngine
{
	namespace NetworkSystem
	{
		enum SERVER_STATUS
		{
			SERVER_STOP,
			SERVER_INITIALZE,
			SERVER_READY,
		};

		class Server
		{
		protected:
			char m_ip[16];
			int	 m_port;
			int	 m_workerThreadCount;

			SERVER_STATUS m_status;
			System::ContentsProcess* m_pContentsProcess;

		public:
			Server(System::ContentsProcess *contentsProcess);
			virtual ~Server();

			virtual HRESULT Init();
			void Release();

			void					putPackage(Package *package);

			virtual bool			run() = 0;

		public:
			//getÇÔ¼ö
			const SERVER_STATUS& status() const { return m_status; }
		};
	}
}
