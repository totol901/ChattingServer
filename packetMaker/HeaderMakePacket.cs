using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace packetMaker
{
    class HeaderMakePacket : MakePacket
    {
        //파싱에 쓰일 변수들
        private string header;
        private string _enum;
        private string tailer;

        private int packetIndexStart = 0;
        private string packetMakeDate;

        //헤더파일의 시트를 파싱하여 header,_enum,tailer 초기화함
        public HeaderMakePacket(string filePath, string formSheet)
            : base(filePath, formSheet)
        {
        }

        //excel_Data에 따라 header, _enum, tailer 초기화함
        protected override void Init(DataTable excel_Data)
        {
            foreach(DataRow row in excel_Data.Rows)
            {
                const int titleIdx = 0;
                const int parseIdx = 1;
                string parseToken = row[titleIdx].ToString();

                if(parseToken.CompareTo("header") == 0)
                {
                    header = row[parseIdx].ToString();
                }
                else if(parseToken.CompareTo("enum") == 0)
                {
                    _enum = row[parseIdx].ToString();
                }
                else if(parseToken.CompareTo("tailer") == 0)
                {
                    tailer = row[parseIdx].ToString();
                }
            }
        }

        //header, _enum, tailer를 합친 코드를 리턴함
        protected override string Parse()
        {
            //header를 파싱
            packetMakeDate = DateTime.Now.ToString("yyyy/mm/dd HH:mm:ss");
            string parseStr = string.Format(header, packetMakeDate) + nextLine;

            //_enum를 파싱
            int index = packetIndexStart;
            foreach(DataRow classRow in excelData.Rows)
            {
                string nameToken = classRow[0].ToString();
                if(nameToken.StartsWith("#"))
                {
                    continue;
                }
                const int classNameCol = 0;

                parseStr += string.Format(_enum, index++, classRow[classNameCol].ToString());
                parseStr += nextLine;
            }

            //tailer를 파싱
            parseStr += tailer;

            //코드 리턴
            return parseStr;
        }
    }
}

