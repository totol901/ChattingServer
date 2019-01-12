#include "stdafx.h"
#include "Package.h"

namespace ServerEngine
{
	namespace NetworkSystem
	{
		Package::Package(Session* pSession, Packet* pPacket)
			:m_pSession(pSession),
			m_pPacket(pPacket)
		{
		}

		Package::~Package()
		{
			m_pSession = nullptr;
			SAFE_DELETE(m_pPacket);
		}
	}
}
