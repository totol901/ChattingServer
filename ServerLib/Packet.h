/******************************************************************************
파일명	: Pakcet.h
목적		: 패킷 구조체와 페킷 타입 정의
******************************************************************************/
#pragma once

namespace ServerEngine
{
	namespace NetworkSystem
	{
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
			PK_ANS_EXIT,
			PK_SEND_MOVE_START,
			PK_RECV_MOVE_START,
			PK_SEND_MOVE_END,
			PK_RECV_MOVE_END,
			PK_SEND_LOCATION_RENEWAL,
			PK_RECV_LOCATION_RENEWAL
		};

		class T_PACKET
		{
		public:
			UINT Size;
			E_PACKET_TYPE type;
			char buff[PAKCET_BUFF_SIZE];
		private:
			static BOOL m_AllocatorOn;

		public:
			T_PACKET();
			T_PACKET(E_PACKET_TYPE _type);
			~T_PACKET();

			void SetStream(Stream& stream)
			{
				memcpy(buff, stream.data(), stream.size());
				Size = (UINT)(sizeof(Size) + stream.size() + sizeof(type));
			}
			void Clear()
			{
				Size = sizeof(*this);
				type = PK_NONE;
				memset(buff, 0, (PAKCET_BUFF_SIZE));
			}

			static void* operator new(size_t allocSize);
			static void operator delete(void* deletepointer);
		};
	}
}