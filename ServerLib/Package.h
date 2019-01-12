#pragma once

namespace ServerEngine
{
	namespace NetworkSystem
	{
		class Package
		{
		public:
			Session* m_pSession;
			Packet* m_pPacket;

			Package(Session* pSession, Packet* pPacket);
			~Package();
		};
	}
}
