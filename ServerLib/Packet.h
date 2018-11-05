#pragma once

enum E_PACKET_TYPE
{
	PK_NONE,
	PK_REQ_LOGIN,
	PK_ANS_LOGIN,
	PK_REQ_CREATE_ID,
	PK_ANS_CREATE_ID,
	PK_REQ_WAITINGCHANNAL_ENTER,
	PK_ANS_WAITINGCHANNAL_ENTER,
	PK_REQ_WAITINGCHANNAL_CHREAT_CHANNAL,
	PK_ANS_WAITINGCHANNAL_CHREAT_CHANNAL,
	PK_REQ_WAITINGCHANNAL_CHANNAL_JOIN,
	PK_ANS_WAITINGCHANNAL_CHANNAL_JOIN,
	PK_REQ_CHANNAL_SEND_MESSAGE,
	PK_RECV_CHANNAL_MESSAGE,
	PK_REQ_CHANNAL_OUT,
	PK_ANS_CHANNAL_OUT,
	PK_REQ_EXIT,
	PK_ANS_EXIT
};

#pragma pack(push, 1)
struct T_PACKET
{
	DWORD Size;
	E_PACKET_TYPE type;
	char buff[PAKCET_BUFF_SIZE];

	T_PACKET()
		:Size(0),
		type(PK_NONE)
	{
		memset(buff, 0, (PAKCET_BUFF_SIZE));
	}
	T_PACKET(E_PACKET_TYPE _type)
	{
		Size = sizeof(*this);
		type = _type;
		memset(buff, 0, (PAKCET_BUFF_SIZE));
	}
	inline void SetStream(Stream& stream)
	{
		memcpy(buff, stream.data(), stream.size());
	}
};
#pragma pack(pop)
