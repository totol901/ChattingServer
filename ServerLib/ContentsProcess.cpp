#include "stdafx.h"
#include "ContentsProcess.h"

namespace ServerEngine
{
	namespace System
	{
		ContentsProcess::ContentsProcess()
		{
		}

		ContentsProcess::~ContentsProcess()
		{
		}

		HRESULT ContentsProcess::Init()
		{
			tinyxml2::XMLDocument config;

			if (!Util::loadConfig(&config))
			{
				return E_FAIL;
			}

			tinyxml2::XMLElement* root = config.FirstChildElement("App")->FirstChildElement("Contents");
			if (!root)
			{
				SLogPrintAtFile(L"@ not exist task setting");
				return E_FAIL;
			}

			tinyxml2::XMLElement* elem = root->FirstChildElement("ThreadCount");
			int processCount = 0;
			sscanf_s(elem->GetText(), "%d", &processCount);

			if (MAX_PACKET_THREAD < processCount)
			{
				SLogPrintAtFile(L"! processThread limit[%d], but config setting [%d]", MAX_PACKET_THREAD, processCount);
				return S_OK;
			}

			m_pPackageQueue = new ThreadJobQueue<NetworkSystem::Package*>((WCHAR*)L"ContentsProcessQueue");
			for (int i = 0; i < processCount; ++i)
			{
				m_ThreadPool[i] = new Thread(this, process);
			}
			this->registDefaultPacketFunc();

			return S_OK;
		}

		void ContentsProcess::Release()
		{
			SAFE_DELETE(m_pPackageQueue);

			for (auto thread : m_ThreadPool)
			{
				SAFE_DELETE(thread);
			}
			m_runFuncTable.clear();
		}

		unsigned int __stdcall ContentsProcess::process(void * param)
		{
			ContentsProcess* temp = (ContentsProcess*)param;
			while (Util::GlobalValue::GetInstance()->m_IsShutdown == false)
			{
				temp->execute();
				CONTEXT_SWITCH;
			}

			return 0;
		}

		void ContentsProcess::putPackage(NetworkSystem::Package *package)
		{
			m_pPackageQueue->push(package);
		}

		void ContentsProcess::Packet_HeartBeat(NetworkSystem::Session * session, NetworkSystem::Packet * rowPacket)
		{
			if (session->GetType() != NetworkSystem::SESSION_TYPE_CLIENT)
			{
				return;
			}

			session->UpdateHeartBeat();
		}

		void ContentsProcess::Packet_Notify_Terminal(NetworkSystem::Session * session, NetworkSystem::Packet * rowPacket)
		{
			session->SetType(NetworkSystem::SESSION_TYPE_TERMINAL);
			
			SLogPrintAtFile("* [%s] Terminal accepted.", inet_ntoa(session->GetClientAddr().sin_addr));
		}

		void ContentsProcess::C_REQ_EXIT(NetworkSystem::Session * session, NetworkSystem::Packet * rowPacket)
		{
			//클라이언트 read thread 를 종료시켜 주기 위해 처리
			NetworkSystem::PK_REQ_EXIT *packet = (NetworkSystem::PK_REQ_EXIT *)rowPacket;

			NetworkSystem::PK_ANS_EXIT ansPacket;

			SLogPrintAtFile("* recv exit packet by [%s]", inet_ntoa(session->GetClientAddr().sin_addr));
			session->SendPacket(&ansPacket);
		}

		void ContentsProcess::registDefaultPacketFunc()
		{
			m_runFuncTable.insert(make_pair(NetworkSystem::E_PK_RECV_HARTBEAT, &ContentsProcess::Packet_HeartBeat));
			m_runFuncTable.insert(make_pair(NetworkSystem::E_PK_I_NOTIFY_TERMINAL, &ContentsProcess::Packet_Notify_Terminal));
			m_runFuncTable.insert(make_pair(NetworkSystem::E_PK_ANS_EXIT, &ContentsProcess::C_REQ_EXIT));
		}

		void ContentsProcess::run(NetworkSystem::Package *package)
		{
			NetworkSystem::E_PACKET_TYPE type = package->m_pPacket->Type();
			auto itr = m_runFuncTable.find(type);
			if (itr == m_runFuncTable.end())
			{
				SLogPrintAtFile(L"! invaild packet runFunction. type[%d]", type);
				//package->m_pSession->->onClose();
				return;
			}
			RunFunc runFunction = itr->second;
#ifdef _DEBUG
			SLogPrintAtFile(L"*** [%d] packet run ***", type);
#endif //_DEBUG
			runFunction(package->m_pSession, package->m_pPacket);
		}

		void ContentsProcess::execute()
		{
			NetworkSystem::Package *package = nullptr;
			if (m_pPackageQueue->pop(package) == false)
			{
				return;
			}
			
			this->run(package);

			SAFE_DELETE(package);
		}
	}
}
