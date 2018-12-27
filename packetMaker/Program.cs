using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace packetMaker
{
    class Program
    {
        static readonly string serverPath = "C:\\Users\\totol\\Desktop\\ChattingServer\\ServerLib";
        static readonly string clientPath = "C:\\Users\\totol\\Desktop\\유니티라니!\\Overflow\\Assets\\Scripts\\Server";

        static readonly string excelPath = "C:\\Users\\totol\\Desktop\\ChattingServer\\참고문서\\packetData1.xlsx";
        static readonly string dataSheet = "packetData";

        static void Main(string[] args)
        {
            Console.WriteLine("* packet maker run!!! ");

            //------server
            //서버 패킷 클래스
            ClassMakePacket classPacket = new ClassMakePacket(excelPath, "packetClass");
            classPacket.Make(dataSheet, serverPath + "\\Packet.h");

            //서버 헤더
            HeaderMakePacket ServerHeader = new HeaderMakePacket(excelPath, "packetHeader");
            ServerHeader.Make(dataSheet, serverPath + "\\PacketHeader.h");

            //서버 팩토리 클래스
            FactoryMakePacket FactoryClass = new FactoryMakePacket(excelPath, "packetFactory");
            FactoryClass.Make(dataSheet, serverPath + "\\PacketFactory.h");


            //------clinet
            //클라이언트 패킷 클래스
            ClientMakePacket clientMakePacket = new ClientMakePacket(excelPath, "clientPacketClass");
            clientMakePacket.Make(dataSheet, clientPath + "\\CPacketClass.cs");

            //클라이언트 헤더
            HeaderMakePacket clientHeader = new HeaderMakePacket(excelPath, "clientPacketHeader");
            clientHeader.Make(dataSheet, clientPath + "\\CPacketHeader.cs");

            //클라 팩토리 클래스
            FactoryMakePacket clientFactory = new FactoryMakePacket(excelPath, "clientPacketFactory");
            clientFactory.Make(dataSheet, clientPath + "\\CPacketFactory.cs");
        }
    }
}
