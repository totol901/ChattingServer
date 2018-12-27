using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Data;
using System.Data.OleDb;
using System.IO;

namespace packetMaker
{
    class ExcelParser
    {
        private DataTable excelTable = null;
        private string provider;

        //엑셀 파일을 파싱함
        public ExcelParser(string filePath, string sheetName)
        {
            this.Read(filePath, sheetName);
        }

        //엑셀 파일 타입 리턴함
        private static int ExcelFileType(string XlsFile)
        {
            byte[,] ExcelHeader =
            {
                {0xD0, 0xCF, 0x11, 0xE0, 0xA1 },//XLS 파일 헤더
                {0x50,0x4B,0x03,0x04,0x14 }//XLSX 파일 헤더
            };

            //result -2=error, -1=not excel, 0 =xls, 1=xlsx
            int result = -1;
            FileInfo FI = new FileInfo(XlsFile);
            FileStream FS = FI.Open(FileMode.Open);
            try
            {
                byte[] FH = new byte[5];
                FS.Read(FH, 0, 5);
                for(int i = 0; i < 2; i++)
                {
                    for (int j = 0; j < 5; j++)
                    {
                        if (FH[j] != ExcelHeader[i, j])
                        {
                            break;
                        }
                        else if (j == 4)
                        {
                            result = i;
                        }
                    }
                    if (result >= 0)
                    {
                        break;
                    }
                }
            }
            catch
            {
                result = -2;
            }
            finally
            {
                FS.Close();
            }
            return result;
        }

        //해당 엑셀파일의 시트 읽기 excelTable에 결과 넣어줌
        private void Read(string filePath, string sheetName)
        {
            //해당 엑셀 파일 프로바이더 설정
            switch(ExcelFileType(filePath))
            {
                case -2:
                    Console.WriteLine("파일 read 오류");
                    return;
                case -1:
                    Console.WriteLine("엑셀 파일 형식 아님");
                    return;
                case 0:
                    provider = "Provider=Microsoft.Jet.OLEDB.4.0; Data Source=" + filePath + ";Extended Properties=Excel 8.0";
                    break;
                case 1:
                    provider = "Provider=Microsoft.ACE.OLEDB.12.0; Data Source=" + filePath + ";Extended properties=Excel 12.0";
                    break;
            }
            
            //OleDb 커넥션 사용하여 파일 오픈
            OleDbConnection excel = new OleDbConnection(provider);
            excel.Open();

            //쿼리 선언
            string strQuery = "SELECT * FROM [" + sheetName + "$]";

            //쿼리 적용
            OleDbCommand dbCommand = new OleDbCommand(strQuery, excel);
            OleDbDataAdapter dataAdapter = new OleDbDataAdapter(dbCommand);

            //쿼리로 나온 데이터 table에 넣어줌
            DataTable table = new DataTable();
            dataAdapter.Fill(table);

            //멤버변수 table에 넣어줌
            excelTable = table;

            //해제
            dataAdapter.Dispose();
            dbCommand.Dispose();

            //해제
            excel.Close();
            excel.Dispose();
        }

        ~ExcelParser()
        {
            excelTable.Dispose();
        }

        public DataTable Table()
        {
            return excelTable;
        }
    }
}
