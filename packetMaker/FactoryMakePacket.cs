using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Data;

namespace packetMaker
{
    class FactoryMakePacket : MakePacket
    {
        private string header;
        private string macro;
        private string tailer;
        
        //엑셀 파일 파싱 준비 및 초기화
        public FactoryMakePacket(string filePath, string formSheet)
            :base(filePath, formSheet)
        {
        }

        //변수 초기화
        protected override void Init(DataTable excel_Data)
        {
            foreach(DataRow datarow in excel_Data.Rows)
            {
                const int titleIdx = 0;
                const int parseIdx = 1;

                string parseStr = datarow[titleIdx].ToString();
                if(parseStr.CompareTo("header") == 0)
                {
                    header = datarow[parseIdx].ToString();
                }
                else if(parseStr.CompareTo("macro") == 0)
                {
                    macro = datarow[parseIdx].ToString();
                }
                else if(parseStr.CompareTo("tailer") == 0)
                {
                    tailer = datarow[parseIdx].ToString();
                }
            }
        }

        //매크로 부분을 파싱함
        private string Macro()
        {
            string parseStr = "";

            foreach(DataRow datarow in excelData.Rows)
            {
                string nameToken = datarow[0].ToString();
                //주석은 건너뜀
                if (nameToken.StartsWith("#"))
                {
                    continue;
                }
                const int classNameCol = 0;

                parseStr += string.Format(macro, datarow[classNameCol].ToString());
                parseStr += nextLine;
            }

            return parseStr;
        }

        //팩토리 시트 파싱
        protected override string Parse()
        {
            string parseStr = "";

            //헤더 파싱
            parseStr += header + nextLine;

            //메크로 파싱
            parseStr += Macro();

            //끝부분 파싱
            parseStr += tailer;

            return parseStr;
        }
    }
}
