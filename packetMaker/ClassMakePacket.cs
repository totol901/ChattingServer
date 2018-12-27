using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace packetMaker
{
    class ClassMakePacket : MakePacket
    {
        private string header;
        private string classHeader;
        private string packetType;
        private string encode;
        private string streamIn;
        private string decode;
        private string streamOut;

        public ClassMakePacket(string filePath, string formSheet)
            :base(filePath, formSheet)
        {
        }

        //변수들 초기화
        //해당 폼의 코드를 넣어줌
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
                else if(parseToken.CompareTo("classHeader") == 0)
                {
                    classHeader = row[parseIdx].ToString();
                }
                else if(parseToken.CompareTo("packetType") == 0)
                {
                    packetType = row[parseIdx].ToString();
                }
                else if(parseToken.CompareTo("encode") == 0)
                {
                    encode = row[parseIdx].ToString();
                }
                else if(parseToken.CompareTo("streamIn") == 0)
                {
                    streamIn = row[parseIdx].ToString();
                }
                else if(parseToken.CompareTo("decode") == 0)
                {
                    decode = row[parseIdx].ToString();
                }
                else if(parseToken.CompareTo("streamOut") == 0)
                {
                    streamOut = row[parseIdx].ToString();
                }
            }
        }

        //packetData의 멤버변수 파싱
        private string ParseMemberVal(DataRow classRow)
        {
            string parseStr = "";

            int columIdx = 0;
            foreach(DataColumn Col in excelData.Columns)
            {
                //0번열 제외
                if(columIdx++ == 0)
                {
                    continue;
                }

                //해당 행열을 토큰으로 넣어줌
                string token = classRow[Col].ToString();
                //비었다면 파싱 끝
                if(token == "")
                {
                    break;
                }

                //변수명인지 변수이름인지 판별
                bool valueString = System.Convert.ToBoolean(columIdx % 2);
                parseStr += tab + tab + tab + tab + token;
                //변수이름이면 ; 붙이고 라인변경
                if (valueString)
                {
                    parseStr += ";" + nextLine;
                }
                //변수명이면 띄워써줌
                else
                {
                    parseStr += " ";
                }
            }

            return parseStr;
        }

        //함수 파싱
        private string ParseFunction(DataRow classRow, string streamStr)
        {
            string parseStr = "";
            int columIndx = 0;
            string typeToken = "";

            foreach(DataColumn Col in excelData.Columns)
            {
                //홀수나 1번열일 때 (변수값) 토큰 설정
                if((columIndx++ % 2) != 0 || columIndx == 1)
                {
                    typeToken = classRow[Col].ToString();
                    continue;
                }

                //짝수 일때 변수이름 valToken 설정
                string valToken = classRow[Col].ToString();
                //변수값이 비었다면 종료
                if(valToken == "")
                {
                    break;
                }

                //변수이름 포멧에 맞게 넣어줌
                parseStr += string.Format(streamStr, valToken);
                parseStr += nextLine;
            }

            return parseStr;
        }

        //인코드 파싱
        private string ParseEncode(DataRow classRow)
        {
            //0,0에 있는 타입 넣어줌
            string enumType = classRow[0].ToString();

            //encode 포멧에 맞게 enumType넣어줌
            string parseStr = nextLine;
            parseStr += string.Format(encode, enumType) + nextLine;
            parseStr += ParseFunction(classRow, streamIn);
            parseStr += tab+ tab + tab + tab + scopeOut + nextLine;

            return parseStr;
        }

        //디코드 파싱
        private string ParseDecode(DataRow classRow)
        {
            //디코드 함수 만듬
            string parseStr = nextLine;
            parseStr += decode + nextLine;
            parseStr += tab+ tab + tab + tab + scopeIn;
            parseStr += ParseFunction(classRow, streamOut);
            parseStr += tab+tab + tab + tab + scopeOut + nextLine;

            return parseStr;
        }

        //파싱 함수
        protected override string Parse()
        {
            //클래스 시작 매크로
            string classStatrMacro = classHeader + nextLine
                                     + tab + tab + scopeIn + nextLine
                                    + packetType + nextLine;
            //클래스 끝 매크로
            string classEndMacro = tab + tab + scopeOut + ";" + nextLine + nextLine;

            string parseStr = header + nextLine + nextLine;
            foreach (DataRow classRow in excelData.Rows)
            {
                //행의 첫번째 token으로 넣어줌
                string nameToken = classRow[0].ToString();
                //#은 제외
                if (nameToken.StartsWith("#"))
                {
                    continue;
                }

                const int classNameCol = 0;
                const int valTokenCol = 1;

                //classStatrMacro 포멧에 맞게 해당 열의 클래스 이름값을 넣어줌
                parseStr += string.Format(classStatrMacro, classRow[classNameCol].ToString());
                //valToken으로 변수명을 넣어줌
                string valToken = classRow[valTokenCol].ToString();
                //없으면 끝
                if (valToken == "")
                {
                    parseStr += classEndMacro;
                    continue;
                }

                //아래의 함수등을 완성하고 클래스를 string에 넣어줌
                parseStr += ParseMemberVal(classRow);
                parseStr += ParseEncode(classRow);
                parseStr += ParseDecode(classRow);
                parseStr += classEndMacro;
            }

            parseStr += tab + scopeOut + nextLine;
            parseStr += scopeOut;

            return parseStr;
        }
    }
}
