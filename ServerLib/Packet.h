#pragma once

namespace ServerEngine
{
    namespace NetworkSystem
    {
        class Packet
        {
        public:
            Packet() {}
            virtual ~Packet() {}

            virtual E_PACKET_TYPE Type() = 0;

            virtual void Encode(SendStream& sendStream)
            {
            sendStream.write(this->Type());
            }

            virtual void Decode(RecvStream& recvStream)
            {
            }
        };

        class PK_NONE : public Packet
      {
        public:
           E_PACKET_TYPE Type()  {  return E_PK_NONE;  }
            string             test;

            void Encode(SendStream& sendStream)
            {
                sendStream.write(this->Type());
                sendStream.write(test);
            }

            void Decode(RecvStream& recvStream)
            {                recvStream.read(&test);
            }
      };

        class PK_REQ_LOGIN : public Packet
      {
        public:
           E_PACKET_TYPE Type()  {  return E_PK_REQ_LOGIN;  }
            string             ID;
            string             PW;

            void Encode(SendStream& sendStream)
            {
                sendStream.write(this->Type());
                sendStream.write(ID);
                sendStream.write(PW);
            }

            void Decode(RecvStream& recvStream)
            {                recvStream.read(&ID);
                recvStream.read(&PW);
            }
      };

        class PK_ANS_LOGIN : public Packet
      {
        public:
           E_PACKET_TYPE Type()  {  return E_PK_ANS_LOGIN;  }
            bool             IsSuccess;
            UInt32             ErrorNumber;
            string             Nickname;

            void Encode(SendStream& sendStream)
            {
                sendStream.write(this->Type());
                sendStream.write(IsSuccess);
                sendStream.write(ErrorNumber);
                sendStream.write(Nickname);
            }

            void Decode(RecvStream& recvStream)
            {                recvStream.read(&IsSuccess);
                recvStream.read(&ErrorNumber);
                recvStream.read(&Nickname);
            }
      };

        class PK_REQ_CREATE_ID : public Packet
      {
        public:
           E_PACKET_TYPE Type()  {  return E_PK_REQ_CREATE_ID;  }
            string             ID;
            string             PW;
            string             Nickname;

            void Encode(SendStream& sendStream)
            {
                sendStream.write(this->Type());
                sendStream.write(ID);
                sendStream.write(PW);
                sendStream.write(Nickname);
            }

            void Decode(RecvStream& recvStream)
            {                recvStream.read(&ID);
                recvStream.read(&PW);
                recvStream.read(&Nickname);
            }
      };

        class PK_ANS_CREATE_ID : public Packet
      {
        public:
           E_PACKET_TYPE Type()  {  return E_PK_ANS_CREATE_ID;  }
            bool             IsSuccess;
            UInt32             ErrorNumber;

            void Encode(SendStream& sendStream)
            {
                sendStream.write(this->Type());
                sendStream.write(IsSuccess);
                sendStream.write(ErrorNumber);
            }

            void Decode(RecvStream& recvStream)
            {                recvStream.read(&IsSuccess);
                recvStream.read(&ErrorNumber);
            }
      };

        class PK_ANS_WAITINGCHANNAL_ENTER : public Packet
      {
        public:
           E_PACKET_TYPE Type()  {  return E_PK_ANS_WAITINGCHANNAL_ENTER;  }
            bool             IsSuccess;
            UInt32             ErrorNumber;
            string             ChannalList;

            void Encode(SendStream& sendStream)
            {
                sendStream.write(this->Type());
                sendStream.write(IsSuccess);
                sendStream.write(ErrorNumber);
                sendStream.write(ChannalList);
            }

            void Decode(RecvStream& recvStream)
            {                recvStream.read(&IsSuccess);
                recvStream.read(&ErrorNumber);
                recvStream.read(&ChannalList);
            }
      };

        class PK_REQ_WAITINGCHANNAL_CHREAT_CHANNAL : public Packet
      {
        public:
           E_PACKET_TYPE Type()  {  return E_PK_REQ_WAITINGCHANNAL_CHREAT_CHANNAL;  }
            string             ChannalName;

            void Encode(SendStream& sendStream)
            {
                sendStream.write(this->Type());
                sendStream.write(ChannalName);
            }

            void Decode(RecvStream& recvStream)
            {                recvStream.read(&ChannalName);
            }
      };

        class PK_ANS_WAITINGCHANNAL_CHREAT_CHANNAL : public Packet
      {
        public:
           E_PACKET_TYPE Type()  {  return E_PK_ANS_WAITINGCHANNAL_CHREAT_CHANNAL;  }
            bool             IsSuccess;
            UInt32             ErrorNumber;

            void Encode(SendStream& sendStream)
            {
                sendStream.write(this->Type());
                sendStream.write(IsSuccess);
                sendStream.write(ErrorNumber);
            }

            void Decode(RecvStream& recvStream)
            {                recvStream.read(&IsSuccess);
                recvStream.read(&ErrorNumber);
            }
      };

        class PK_REQ_WAITINGCHANNAL_CHANNAL_JOIN : public Packet
      {
        public:
           E_PACKET_TYPE Type()  {  return E_PK_REQ_WAITINGCHANNAL_CHANNAL_JOIN;  }
            UInt32             ChannalNumber;

            void Encode(SendStream& sendStream)
            {
                sendStream.write(this->Type());
                sendStream.write(ChannalNumber);
            }

            void Decode(RecvStream& recvStream)
            {                recvStream.read(&ChannalNumber);
            }
      };

        class PK_ANS_WAITINGCHANNAL_CHANNAL_JOIN : public Packet
      {
        public:
           E_PACKET_TYPE Type()  {  return E_PK_ANS_WAITINGCHANNAL_CHANNAL_JOIN;  }
            bool             IsSuccess;
            UInt32             ErrorNumber;

            void Encode(SendStream& sendStream)
            {
                sendStream.write(this->Type());
                sendStream.write(IsSuccess);
                sendStream.write(ErrorNumber);
            }

            void Decode(RecvStream& recvStream)
            {                recvStream.read(&IsSuccess);
                recvStream.read(&ErrorNumber);
            }
      };

        class PK_REQ_CHANNAL_SEND_MESSAGE : public Packet
      {
        public:
           E_PACKET_TYPE Type()  {  return E_PK_REQ_CHANNAL_SEND_MESSAGE;  }
            string             Message;

            void Encode(SendStream& sendStream)
            {
                sendStream.write(this->Type());
                sendStream.write(Message);
            }

            void Decode(RecvStream& recvStream)
            {                recvStream.read(&Message);
            }
      };

        class PK_RECV_CHANNAL_MESSAGE : public Packet
      {
        public:
           E_PACKET_TYPE Type()  {  return E_PK_RECV_CHANNAL_MESSAGE;  }
            string             Nickname;
            string             Message;

            void Encode(SendStream& sendStream)
            {
                sendStream.write(this->Type());
                sendStream.write(Nickname);
                sendStream.write(Message);
            }

            void Decode(RecvStream& recvStream)
            {                recvStream.read(&Nickname);
                recvStream.read(&Message);
            }
      };

        class PK_REQ_CHANNAL_OUT : public Packet
      {
        public:
           E_PACKET_TYPE Type()  {  return E_PK_REQ_CHANNAL_OUT;  }
      };

        class PK_ANS_CHANNAL_OUT : public Packet
      {
        public:
           E_PACKET_TYPE Type()  {  return E_PK_ANS_CHANNAL_OUT;  }
            bool             IsSuccess;
            UInt32             ErrorNumber;

            void Encode(SendStream& sendStream)
            {
                sendStream.write(this->Type());
                sendStream.write(IsSuccess);
                sendStream.write(ErrorNumber);
            }

            void Decode(RecvStream& recvStream)
            {                recvStream.read(&IsSuccess);
                recvStream.read(&ErrorNumber);
            }
      };

        class PK_REQ_EXIT : public Packet
      {
        public:
           E_PACKET_TYPE Type()  {  return E_PK_REQ_EXIT;  }
      };

        class PK_ANS_EXIT : public Packet
      {
        public:
           E_PACKET_TYPE Type()  {  return E_PK_ANS_EXIT;  }
            bool             IsSuccess;
            UInt32             ErrorNumber;

            void Encode(SendStream& sendStream)
            {
                sendStream.write(this->Type());
                sendStream.write(IsSuccess);
                sendStream.write(ErrorNumber);
            }

            void Decode(RecvStream& recvStream)
            {                recvStream.read(&IsSuccess);
                recvStream.read(&ErrorNumber);
            }
      };

        class PK_SEND_MOVE_START : public Packet
      {
        public:
           E_PACKET_TYPE Type()  {  return E_PK_SEND_MOVE_START;  }
            float             LocationX;
            float             LocationY;
            float             DirectionX;
            float             DirectionY;

            void Encode(SendStream& sendStream)
            {
                sendStream.write(this->Type());
                sendStream.write(LocationX);
                sendStream.write(LocationY);
                sendStream.write(DirectionX);
                sendStream.write(DirectionY);
            }

            void Decode(RecvStream& recvStream)
            {                recvStream.read(&LocationX);
                recvStream.read(&LocationY);
                recvStream.read(&DirectionX);
                recvStream.read(&DirectionY);
            }
      };

        class PK_RECV_MOVE_START : public Packet
      {
        public:
           E_PACKET_TYPE Type()  {  return E_PK_RECV_MOVE_START;  }
            string             ID;
            float             LocationX;
            float             LocationY;
            float             DirectionX;
            float             DirectionY;
            float             Velocity;

            void Encode(SendStream& sendStream)
            {
                sendStream.write(this->Type());
                sendStream.write(ID);
                sendStream.write(LocationX);
                sendStream.write(LocationY);
                sendStream.write(DirectionX);
                sendStream.write(DirectionY);
                sendStream.write(Velocity);
            }

            void Decode(RecvStream& recvStream)
            {                recvStream.read(&ID);
                recvStream.read(&LocationX);
                recvStream.read(&LocationY);
                recvStream.read(&DirectionX);
                recvStream.read(&DirectionY);
                recvStream.read(&Velocity);
            }
      };

        class PK_SEND_MOVE_END : public Packet
      {
        public:
           E_PACKET_TYPE Type()  {  return E_PK_SEND_MOVE_END;  }
            float             LocationX;
            float             LocationY;
            float             DirectionX;
            float             DirectionY;

            void Encode(SendStream& sendStream)
            {
                sendStream.write(this->Type());
                sendStream.write(LocationX);
                sendStream.write(LocationY);
                sendStream.write(DirectionX);
                sendStream.write(DirectionY);
            }

            void Decode(RecvStream& recvStream)
            {                recvStream.read(&LocationX);
                recvStream.read(&LocationY);
                recvStream.read(&DirectionX);
                recvStream.read(&DirectionY);
            }
      };

        class PK_RECV_MOVE_END : public Packet
      {
        public:
           E_PACKET_TYPE Type()  {  return E_PK_RECV_MOVE_END;  }
            string             ID;
            float             LocationX;
            float             LocationY;
            float             DirectionX;
            float             DirectionY;

            void Encode(SendStream& sendStream)
            {
                sendStream.write(this->Type());
                sendStream.write(ID);
                sendStream.write(LocationX);
                sendStream.write(LocationY);
                sendStream.write(DirectionX);
                sendStream.write(DirectionY);
            }

            void Decode(RecvStream& recvStream)
            {                recvStream.read(&ID);
                recvStream.read(&LocationX);
                recvStream.read(&LocationY);
                recvStream.read(&DirectionX);
                recvStream.read(&DirectionY);
            }
      };

        class PK_SEND_LOCATION_RENEWAL : public Packet
      {
        public:
           E_PACKET_TYPE Type()  {  return E_PK_SEND_LOCATION_RENEWAL;  }
            float             LocationX;
            float             LocationY;
            float             DirectionX;
            float             DirectionY;

            void Encode(SendStream& sendStream)
            {
                sendStream.write(this->Type());
                sendStream.write(LocationX);
                sendStream.write(LocationY);
                sendStream.write(DirectionX);
                sendStream.write(DirectionY);
            }

            void Decode(RecvStream& recvStream)
            {                recvStream.read(&LocationX);
                recvStream.read(&LocationY);
                recvStream.read(&DirectionX);
                recvStream.read(&DirectionY);
            }
      };

        class PK_RECV_LOCATION_RENEWAL : public Packet
      {
        public:
           E_PACKET_TYPE Type()  {  return E_PK_RECV_LOCATION_RENEWAL;  }
            string             ID;
            float             LocationX;
            float             LocationY;
            float             DirectionX;
            float             DirectionY;

            void Encode(SendStream& sendStream)
            {
                sendStream.write(this->Type());
                sendStream.write(ID);
                sendStream.write(LocationX);
                sendStream.write(LocationY);
                sendStream.write(DirectionX);
                sendStream.write(DirectionY);
            }

            void Decode(RecvStream& recvStream)
            {                recvStream.read(&ID);
                recvStream.read(&LocationX);
                recvStream.read(&LocationY);
                recvStream.read(&DirectionX);
                recvStream.read(&DirectionY);
            }
      };

        class PK_SEND_HARTBEAT : public Packet
      {
        public:
           E_PACKET_TYPE Type()  {  return E_PK_SEND_HARTBEAT;  }
      };

        class PK_RECV_HARTBEAT : public Packet
      {
        public:
           E_PACKET_TYPE Type()  {  return E_PK_RECV_HARTBEAT;  }
      };

        class PK_SEND_SERVERTIME : public Packet
      {
        public:
           E_PACKET_TYPE Type()  {  return E_PK_SEND_SERVERTIME;  }
            Int32             ServerTime;

            void Encode(SendStream& sendStream)
            {
                sendStream.write(this->Type());
                sendStream.write(ServerTime);
            }

            void Decode(RecvStream& recvStream)
            {                recvStream.read(&ServerTime);
            }
      };

        class PK_RECV_CHANNAL_JOIN_ANNOUNCE : public Packet
      {
        public:
           E_PACKET_TYPE Type()  {  return E_PK_RECV_CHANNAL_JOIN_ANNOUNCE;  }
            string             ID;

            void Encode(SendStream& sendStream)
            {
                sendStream.write(this->Type());
                sendStream.write(ID);
            }

            void Decode(RecvStream& recvStream)
            {                recvStream.read(&ID);
            }
      };

        class PK_RECV_CHANNAL_DATA : public Packet
      {
        public:
           E_PACKET_TYPE Type()  {  return E_PK_RECV_CHANNAL_DATA;  }
            string             ID;
            float             LocationX;
            float             LocationY;
            float             DirectionX;
            float             DirectionY;
            float             Velocity;

            void Encode(SendStream& sendStream)
            {
                sendStream.write(this->Type());
                sendStream.write(ID);
                sendStream.write(LocationX);
                sendStream.write(LocationY);
                sendStream.write(DirectionX);
                sendStream.write(DirectionY);
                sendStream.write(Velocity);
            }

            void Decode(RecvStream& recvStream)
            {                recvStream.read(&ID);
                recvStream.read(&LocationX);
                recvStream.read(&LocationY);
                recvStream.read(&DirectionX);
                recvStream.read(&DirectionY);
                recvStream.read(&Velocity);
            }
      };

   }
}
