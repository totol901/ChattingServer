using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Data;
using System.Threading.Tasks;

namespace packetMaker
{
    class ClientMakePacket :  MakePacket
    {
        private string header;
        private string classHeader;
        private string classTailer;
        private string encodeFuncHead;
        private string encodeFunction;
        private string decodeFuncHead;
        private string decodeFunction;
        private string tailer;

        //엑셀 파일 파싱 준비 및 초기화
        public ClientMakePacket(string filePath, string formSheet)
            :base(filePath, formSheet)
        {
        }

        //변수 초기화
        protected override void Init(DataTable excel_Data)
        {
            const int titleiIndx = 0;
            const int valIndex = 1;

            foreach(DataRow dataRow in excel_Data.Rows)
            {
                string parseStr = dataRow[titleiIndx].ToString();
                if (parseStr.CompareTo("header") == 0)
                {
                    header = dataRow[valIndex].ToString();
                }
                else if(parseStr.CompareTo("classHeader") == 0)
                {
                    classHeader = dataRow[valIndex].ToString();
                }
                else if(parseStr.CompareTo("classTailer") == 0)
                {
                    classTailer = dataRow[valIndex].ToString();
                }
                else if (parseStr.CompareTo("encodeFuncHead") == 0)
                {
                    encodeFuncHead = dataRow[valIndex].ToString();
                }
                else if (parseStr.CompareTo("encodeFunction") == 0)
                {
                    encodeFunction = dataRow[valIndex].ToString();
                }
                else if (parseStr.CompareTo("decodeFuncHead") == 0)
                {
                    decodeFuncHead = dataRow[valIndex].ToString();
                }
                else if (parseStr.CompareTo("decodeFunction") == 0)
                {
                    decodeFunction = dataRow[valIndex].ToString();
                }
                else if (parseStr.CompareTo("tailer") == 0)
                {
                    tailer = dataRow[valIndex].ToString();
                }
            }
        }

        //맴버변수 파싱
        private string AddMemvalue(DataRow datarow)
        {
            string parseStr = "";

            parseStr += nextLine;

            int ColIndex = 0;
            foreach(DataColumn dataColumn in excelData.Columns)
            {
                //0 제외
                if(ColIndex++ == 0)
                {
                    continue;
                }

                string token = datarow[dataColumn].ToString();
                //열이 짝수번째 일 경우
                if (ColIndex % 2 == 0)
                {
                    //짝수번째에 값이 없으면 파싱할거 없으니 나가줌
                    if(token.CompareTo("") == 0)
                    {
                        break;
                    }

                    parseStr += tab + tab + tab +"public " + token + " ";
                }
                //열이 홀수번째 일 경우
                else
                {
                    parseStr += token + ";" + nextLine;
                }
            }
            parseStr += nextLine;

            return parseStr;
        }

        //encode 함수 파싱
        private string AddEncodeFunc(DataRow datarow)
        {
            string parseStr = "";

            parseStr += encodeFuncHead + nextLine;

            int ColIndex = 0;
            foreach (DataColumn dataColumn in excelData.Columns)
            {
                //0 제외
                if (ColIndex++ == 0)
                {
                    continue;
                }

                string token = datarow[dataColumn].ToString();
                if (token.CompareTo("") == 0)
                {
                    break;
                }

                //열이 짝수번째 일 경우
                if (ColIndex % 2 == 1)
                {
                    parseStr += string.Format(encodeFunction, token) + nextLine;
                }
            }

            parseStr += tab + tab + tab + scopeOut + nextLine;

            return parseStr;
        }

        //decode 함수 파싱
        private string AddDecodeFunc(DataRow datarow)
        {
            string parseStr = "";

            parseStr += nextLine + decodeFuncHead + nextLine;

            int ColIndex = 0;
            foreach (DataColumn dataColumn in excelData.Columns)
            {
                //짝수 제외
                if (ColIndex % 2 == 0)
                {
                    ColIndex++;
                    continue;
                }


                string token = datarow[dataColumn].ToString();
                string token1 = datarow[ColIndex + 1].ToString();

                if(token.CompareTo("") == 0)
                {
                    break;
                }


                parseStr += string.Format(decodeFunction, token1, token) + nextLine;

                ColIndex++;
            }

            parseStr += tab + tab + tab + scopeOut + nextLine;

            return parseStr;
        }

        //매크로 부분을 파싱함
        private string MakeClasses()
        {
            string parseStr = "";

            foreach (DataRow datarow in excelData.Rows)
            {
                string nameToken = datarow[0].ToString();
                //주석은 건너뜀
                if (nameToken.StartsWith("#"))
                {
                    continue;
                }
                const int classNameCol = 0;

                //헤더 작성
                parseStr += string.Format(classHeader, datarow[classNameCol].ToString());
                parseStr += nextLine;

                //변수추가
                parseStr += AddMemvalue(datarow);

                //함수추가
                parseStr += AddEncodeFunc(datarow);
                parseStr += AddDecodeFunc(datarow) + nextLine;

                //ClassTailer 추가
                parseStr += classTailer + nextLine + nextLine;
            }

            return parseStr;
        }

        //클라이언트 패킷 시트 파싱
        protected override string Parse()
        {
            string parseStr = "";

            //헤더 파싱
            parseStr += header + nextLine;

            //메크로 파싱
            parseStr += MakeClasses();

            //끝부분 파싱
            parseStr += tailer;

            return parseStr;
        }
    }
}
