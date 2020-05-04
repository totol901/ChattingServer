#pragma once

class LoginProcess : public System::ContentsProcess
{
private:
	void registSubPacketFunc();

	//void Packet_ID_PW(NetworkSystem::Session* pSession, NetworkSystem::Packet* rowPacket);
	//void IDB_Packet_ID_PW(NetworkSystem::Session* pSession, NetworkSystem::Packet* rowPacket);
	
public:
	LoginProcess();
	~LoginProcess();

	


};

