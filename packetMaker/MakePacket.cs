using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Data;
using System.IO;

namespace packetMaker
{
    class MakePacket
    {
        //split용 변수들
        public string nextLine = "#newLine";
        public string tab = "   ";
        public string scopeIn = "`";
        public string scopeOut = "☆";

        protected string excelFilePath;
        protected DataTable excelData;

        //패킷 만드는 
        public MakePacket(string filePath, string formSheet)
        {
            ExcelParser excelPS = new ExcelParser(filePath, formSheet);
            this.Init(excelPS.Table());

            excelFilePath = filePath;
        }

        //데이터 시트 만들기
        public void Make(string dataSheet, string outFileName)
        {
            ExcelParser excel = new ExcelParser(excelFilePath, dataSheet);
            excelData = excel.Table();
            this.WriteFile(this.Parse(), outFileName);
        }

        protected virtual void Init(DataTable excel_Data)
        {
            Console.WriteLine("MakePacket.Init() override 오류");
        }

        protected virtual string Parse()
        {
            Console.WriteLine("MakePacket.Parse() override 오류");
            return null;
        }

        //parseStr을 outFileName File에 Write해주는 메서드
        private void WriteFile(string parseStr, string outFileName)
        {
            //중괄호는 C언어로 보면 %d, %s 같은 개념이라 파싱할때 못썻으니
            //파일로 쓰기 전에에 임시 중괄호를 진짜 중괄호로 바꾼다.
            parseStr = parseStr.Replace(scopeIn, "{");
            parseStr = parseStr.Replace(scopeOut, "}");

            //파일 쓰기 위한 파일 스트림 선언
            FileStream fs;
            fs = new FileStream(outFileName, FileMode.Create);

            //스트림 라이터 선언
            StreamWriter writer = new StreamWriter(fs);
            //nextLine마다 잘라서 lines 배열에 넣어줌
            string[] lines = System.Text.RegularExpressions.Regex.Split(parseStr, nextLine);
            //라이터를 사용하여 line을 쓰고 콘솔창에도 써줌
            foreach(string line in lines)
            {
                writer.WriteLine(line);
                Console.WriteLine(line);
            }

            //다 썼으니 라이터 종료
            writer.Close();
        }
    }
}
