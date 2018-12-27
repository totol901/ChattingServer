#include "stdafx.h"
#include "Channel.h"

Channel::Channel(const wstring& channelName)
	:Object()
{
	m_ChannelID = CHANNELMANAGER->GetCanMakeMinimumChannelID();
	m_type = OBJECT_CHANNEL;
	m_name = channelName;
}

Channel::~Channel()
{
}

bool Channel::ChannelErase()
{
	if (!IsChannelEmpty())
	{
		return false;
	}

	CHANNELMANAGER->DeleteChannelByID(this->GetChannelID());

	return true;
}

bool Channel::InsertClientSession(ClientSession * clientsession)
{
	auto iter = m_setClientSessions.find(clientsession);
	if (iter == m_setClientSessions.end())
	{
		m_setClientSessions.insert(clientsession);
		return true;
	}
	
	return false;
}

bool Channel::IsClientSessionIntoSet(ClientSession * clientsession)
{
	auto iter = m_setClientSessions.find(clientsession);
	if (iter == m_setClientSessions.end())
	{
		return false;
	}

	return true;
}

bool Channel::DeleteClientSession(ClientSession * clientsession)
{
	auto iter = m_setClientSessions.find(clientsession);
	if (iter == m_setClientSessions.end())
	{
		return false;
	}

	m_setClientSessions.erase(iter);
	return true;
}

void Channel::SendPacketToChannelMember(Stream& packetData)
{
	for (auto iter = m_setClientSessions.begin();
		iter != m_setClientSessions.end();
		iter++)
	{
		(*iter)->SendPacket(packetData);
	}
}

void Channel::SendPacketChannelData(ClientSession* clientsession)
{
	PK_RECV_CHANNAL_DATA packet;
	SendStream sendStream;

	for (auto iter = m_setClientSessions.begin();
		iter != m_setClientSessions.end();
		iter++)
	{
		LoginChattingClientSession* temp = (LoginChattingClientSession*)(*iter);

		//움직인 시간 구해줌
		chrono::high_resolution_clock::time_point currentTime = TIMER->GetNowHighTimePoint();
		chrono::milliseconds mill = chrono::duration_cast<chrono::milliseconds>(
			currentTime - temp->GetPlayerData()->m_DoStartTimePoint);
		double moveTime = mill.count() * 0.001f;

		//움직인 거리
		float moveLength = (float)moveTime * temp->GetPlayerData()->GetVelocity();
		//총 거리
		float Length = Util::GetDistance(temp->GetPlayerData()->GetLocationX(),
			temp->GetPlayerData()->GetLocationY(),
			temp->GetPlayerData()->GetDirectionX(),
			temp->GetPlayerData()->GetDirectionY());

		float resultX = 0.0f;
		float resultY = 0.0f;
		//선형 보간
		if (Length != 0.0f)
		{
			resultX = (1.0f - (moveLength / Length)) * temp->GetPlayerData()->GetLocationX() +
				(moveLength / Length) * temp->GetPlayerData()->GetDirectionX();
			resultY = (1.0f - (moveLength / Length)) * temp->GetPlayerData()->GetLocationY() +
				(moveLength / Length) * temp->GetPlayerData()->GetDirectionY();
		}
		if (moveLength > Length)
		{
			resultX = temp->GetPlayerData()->GetDirectionX();
			resultY = temp->GetPlayerData()->GetDirectionY();
		}

		sendStream.clear();
		CHAR asiiID[32] = { 0, };
		Util::StrConvW2A((WCHAR*)temp->GetPlayerData()->GetPlayerID().c_str(),
			asiiID, sizeof(asiiID));
		packet.ID = asiiID;
		packet.LocationX = resultX;
		packet.LocationY = resultY;
		packet.DirectionX = temp->GetPlayerData()->GetDirectionX();
		packet.DirectionY = temp->GetPlayerData()->GetDirectionY();
		packet.Velocity = temp->GetPlayerData()->GetVelocity();
		packet.Encode(sendStream);

		clientsession->SendPacket(sendStream);

		SLogPrint(L"%s : MoveEnd, resultX : %f, resultY : %f",
			temp->GetPlayerData()->GetPlayerID().c_str(),
			resultX,
			resultY
		);
	}
}
