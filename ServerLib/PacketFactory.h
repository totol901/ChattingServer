#pragma once

namespace ServerEngine
{
    namespace NetworkSystem
    {
        class PacketFactory : public Singleton<PacketFactory>
        {
            friend Singleton;
        public:
            Packet* GetPacket(int packetType)
            {
                 switch(packetType)
                 {
                    case E_PK_NONE: return new PK_NONE();
                    case E_PK_REQ_LOGIN: return new PK_REQ_LOGIN();
                    case E_PK_ANS_LOGIN: return new PK_ANS_LOGIN();
                    case E_PK_REQ_CREATE_ID: return new PK_REQ_CREATE_ID();
                    case E_PK_ANS_CREATE_ID: return new PK_ANS_CREATE_ID();
                    case E_PK_REQ_WAITINGCHANNAL_ENTER: return new PK_REQ_WAITINGCHANNAL_ENTER();
                    case E_PK_ANS_WAITINGCHANNAL_ENTER: return new PK_ANS_WAITINGCHANNAL_ENTER();
                    case E_PK_REQ_WAITINGCHANNAL_CHREAT_CHANNAL: return new PK_REQ_WAITINGCHANNAL_CHREAT_CHANNAL();
                    case E_PK_ANS_WAITINGCHANNAL_CHREAT_CHANNAL: return new PK_ANS_WAITINGCHANNAL_CHREAT_CHANNAL();
                    case E_PK_REQ_WAITINGCHANNAL_CHANNAL_JOIN: return new PK_REQ_WAITINGCHANNAL_CHANNAL_JOIN();
                    case E_PK_ANS_WAITINGCHANNAL_CHANNAL_JOIN: return new PK_ANS_WAITINGCHANNAL_CHANNAL_JOIN();
                    case E_PK_REQ_CHANNAL_SEND_MESSAGE: return new PK_REQ_CHANNAL_SEND_MESSAGE();
                    case E_PK_RECV_CHANNAL_MESSAGE: return new PK_RECV_CHANNAL_MESSAGE();
                    case E_PK_REQ_CHANNAL_OUT: return new PK_REQ_CHANNAL_OUT();
                    case E_PK_ANS_CHANNAL_OUT: return new PK_ANS_CHANNAL_OUT();
                    case E_PK_REQ_EXIT: return new PK_REQ_EXIT();
                    case E_PK_ANS_EXIT: return new PK_ANS_EXIT();
                    case E_PK_SEND_MOVE_START: return new PK_SEND_MOVE_START();
                    case E_PK_RECV_MOVE_START: return new PK_RECV_MOVE_START();
                    case E_PK_SEND_MOVE_END: return new PK_SEND_MOVE_END();
                    case E_PK_RECV_MOVE_END: return new PK_RECV_MOVE_END();
                    case E_PK_SEND_LOCATION_RENEWAL: return new PK_SEND_LOCATION_RENEWAL();
                    case E_PK_RECV_LOCATION_RENEWAL: return new PK_RECV_LOCATION_RENEWAL();
                    case E_PK_SEND_HARTBEAT: return new PK_SEND_HARTBEAT();
                    case E_PK_RECV_HARTBEAT: return new PK_RECV_HARTBEAT();
                    case E_PK_SEND_SERVERTIME: return new PK_SEND_SERVERTIME();
                    case E_PK_RECV_CHANNAL_JOIN_ANNOUNCE: return new PK_RECV_CHANNAL_JOIN_ANNOUNCE();
                    case E_PK_RECV_CHANNAL_OUT_ANNOUNCE: return new PK_RECV_CHANNAL_OUT_ANNOUNCE();
                    case E_PK_RECV_CHANNAL_DATA: return new PK_RECV_CHANNAL_DATA();
                    case E_PK_I_NOTIFY_TERMINAL: return new PK_I_NOTIFY_TERMINAL();
                 }
                 return nullptr;
             }
        };
    }
}
