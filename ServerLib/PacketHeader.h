#pragma once

namespace ServerEngine
{
    namespace NetworkSystem
    {
        inline wstring PacketMakeDate()
        {
             return L"2018-02-24 01:02:28";
        }
        enum E_PACKET_TYPE
        {
              /*0*/    E_PK_NONE = 0,
              /*1*/    E_PK_REQ_LOGIN = 1,
              /*2*/    E_PK_ANS_LOGIN = 2,
              /*3*/    E_PK_REQ_CREATE_ID = 3,
              /*4*/    E_PK_ANS_CREATE_ID = 4,
              /*5*/    E_PK_ANS_WAITINGCHANNAL_ENTER = 5,
              /*6*/    E_PK_REQ_WAITINGCHANNAL_CHREAT_CHANNAL = 6,
              /*7*/    E_PK_ANS_WAITINGCHANNAL_CHREAT_CHANNAL = 7,
              /*8*/    E_PK_REQ_WAITINGCHANNAL_CHANNAL_JOIN = 8,
              /*9*/    E_PK_ANS_WAITINGCHANNAL_CHANNAL_JOIN = 9,
              /*10*/    E_PK_REQ_CHANNAL_SEND_MESSAGE = 10,
              /*11*/    E_PK_RECV_CHANNAL_MESSAGE = 11,
              /*12*/    E_PK_REQ_CHANNAL_OUT = 12,
              /*13*/    E_PK_ANS_CHANNAL_OUT = 13,
              /*14*/    E_PK_REQ_EXIT = 14,
              /*15*/    E_PK_ANS_EXIT = 15,
              /*16*/    E_PK_SEND_MOVE_START = 16,
              /*17*/    E_PK_RECV_MOVE_START = 17,
              /*18*/    E_PK_SEND_MOVE_END = 18,
              /*19*/    E_PK_RECV_MOVE_END = 19,
              /*20*/    E_PK_SEND_LOCATION_RENEWAL = 20,
              /*21*/    E_PK_RECV_LOCATION_RENEWAL = 21,
              /*22*/    E_PK_SEND_HARTBEAT = 22,
              /*23*/    E_PK_RECV_HARTBEAT = 23,
              /*24*/    E_PK_SEND_SERVERTIME = 24,
              /*25*/    E_PK_RECV_CHANNAL_JOIN_ANNOUNCE = 25,
              /*26*/    E_PK_RECV_CHANNAL_DATA = 26,
        };
    }
}
