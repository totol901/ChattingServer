#pragma once

namespace ServerEngine
{
	namespace System
	{
		constexpr auto MAX_PACKET_THREAD = 64;

		class ContentsProcess
		{
		private:
			array<Thread*, MAX_PACKET_THREAD> m_ThreadPool;
			ThreadJobQueue<NetworkSystem::Package*>* m_pPackageQueue;

		protected:
			typedef function<void(NetworkSystem::Session *pSession, 
				NetworkSystem::Packet *pRowPacket)> RunFunc;
			unordered_map<NetworkSystem::E_PACKET_TYPE, RunFunc> runFuncTable_;

		public:
			ContentsProcess();
			~ContentsProcess();

			HRESULT Init();
			void Release();
			static unsigned int WINAPI process(void* param);
			void registDefaultPacketFunc();
			void run(NetworkSystem::Package *package);
			void execute();

			void putPackage(NetworkSystem::Package *package);

			virtual void registSubPacketFunc() = 0;

			//--------------------------------------------------------------//
			// 기본 패킷 기능 구현
			static void Packet_HeartBeat(NetworkSystem::Session *session, NetworkSystem::Packet *rowPacket);
			static void Packet_Notify_Terminal(NetworkSystem::Session *session, NetworkSystem::Packet *rowPacket);
			static void C_REQ_EXIT(NetworkSystem::Session *session, NetworkSystem::Packet *rowPacket);
		};

	}
}
