#include "stdafx.h"
#include "QueryRecord.h"

namespace ServerEngine
{
	namespace DatabaseSystem
	{
		QueryRecord::QueryRecord()
		{
			m_pRecord.CreateInstance(__uuidof(Recordset));
		}

		QueryRecord::~QueryRecord()
		{
			if (m_pRecord == nullptr)
			{
				return;
			}
			
			m_pRecord.Release();
			m_pRecord = nullptr;
		}

		void QueryRecord::errorReport(::_com_error & e)
		{
			SLogPrintAtFile(L"* Query error = %S", e.Description());
		}

		void QueryRecord::operator=(QueryRecord & lvalue)
		{
			m_pRecord = lvalue.resultRecord();
		}

		void QueryRecord::operator=(QueryRecord && rvalue)
		{
			m_pRecord = rvalue.resultRecord();
		}

		void QueryRecord::operator=(::_RecordsetPtr & lvalue)
		{
			m_pRecord = lvalue;
		}

		void QueryRecord::operator=(::_RecordsetPtr && rvalue)
		{
			m_pRecord = rvalue;
		}

		::_RecordsetPtr & QueryRecord::resultRecord()
		{
			return m_pRecord;
		}

		bool QueryRecord::opened()
		{
			return m_pRecord->State == ::adStateOpen ? true : false;
		}

		void QueryRecord::close()
		{
			try 
			{
				if (m_pRecord != nullptr && this->opened())
				{
					m_pRecord->Close();
				}
			}
			catch (_com_error &e) 
			{
				this->errorReport(e);
			}
		}

		void QueryRecord::setRecord(const ::_RecordsetPtr& record)
		{
			m_pRecord = record;
		}

		bool QueryRecord::isEof()
		{
			if (m_pRecord == nullptr)
			{
				return true;
			}

			try 
			{
				return m_pRecord->EndOfFile ? true : false;
			}
			catch (_com_error &e) 
			{
				this->errorReport(e);
			}

			return false;
		}

		int QueryRecord::resultVal()
		{
			return m_resultVal;
		}

		void QueryRecord::setResultVal(const int& result)
		{
			m_resultVal = result;
		}

		HRESULT QueryRecord::moveNext()
		{
			try
			{
				return m_pRecord->MoveNext();
			}
			catch (_com_error &e) 
			{
				this->errorReport(e);
			}

			return S_FALSE;
		}

		HRESULT QueryRecord::movePrevious()
		{
			try 
			{
				return m_pRecord->MovePrevious();
			}
			catch (_com_error &e) 
			{
				this->errorReport(e);
			}

			return S_FALSE;
		}

		HRESULT QueryRecord::moveFirst()
		{
			try 
			{
				return m_pRecord->MoveFirst();
			}
			catch (_com_error &e) 
			{
				this->errorReport(e);
			}

			return S_FALSE;
		}

		HRESULT QueryRecord::moveLast()
		{
			try
			{
				return m_pRecord->MoveLast();
			}
			catch (_com_error &e)
			{
				this->errorReport(e);
			}

			return S_FALSE;
		}

		bool QueryRecord::GetVal(char * fieldName, char * fieldValue)
		{
			try 
			{
				_variant_t  vtValue;
				vtValue = m_pRecord->Fields->GetItem(fieldName)->GetValue();
				sprintf_s(fieldValue, DB_PARAM_SIZE, "%s", (LPCSTR)((_bstr_t)vtValue.bstrVal));
				return true;
			}
			catch (_com_error &e)
			{
				this->errorReport(e);
				SLogPrintAtFile(L"! error query field : %S", fieldName);
			}

			return false;
		}

		bool QueryRecord::GetVal(char * fieldName, wchar_t * fieldValue)
		{
			try 
			{
				_variant_t  vtValue;
				vtValue = m_pRecord->Fields->GetItem(fieldName)->GetValue();
				swprintf_s(fieldValue, DB_PARAM_SIZE, L"%s", (LPWSTR)((_bstr_t)vtValue.bstrVal));
				return true;
			}
			catch (_com_error &e) 
			{
				this->errorReport(e);
				SLogPrintAtFile(L"! error query field : %S", fieldName);
			}

			return false;
		}

		bool QueryRecord::GetVal(char * fieldName, int32_t & fieldValue)
		{
			try 
			{
				_variant_t  vtValue;
				vtValue = m_pRecord->Fields->GetItem(fieldName)->GetValue();
				fieldValue = vtValue.intVal;
				return true;
			}
			catch (_com_error &e) 
			{
				this->errorReport(e);
				SLogPrintAtFile(L"! error query field : %S", fieldName);
			}

			return false;
		}

		bool QueryRecord::GetVal(char * fieldName, int64_t & fieldValue)
		{
			try 
			{
				_variant_t  vtValue;
				vtValue = m_pRecord->Fields->GetItem(fieldName)->GetValue();
				fieldValue = vtValue.llVal;
				return true;
			}
			catch (_com_error &e) 
			{
				this->errorReport(e);
				SLogPrintAtFile(L"! error query field : %S", fieldName);
			}

			return false;
		}

		bool QueryRecord::GetVal(char * fieldName, float & fieldValue)
		{
			try 
			{
				_variant_t  vtValue;
				vtValue = m_pRecord->Fields->GetItem(fieldName)->GetValue();
				fieldValue = vtValue.fltVal;
				return true;
			}
			catch (_com_error &e) 
			{
				this->errorReport(e);
				SLogPrintAtFile(L"! error query field : %S", fieldName);
			}
			return false;
		}

		bool QueryRecord::GetVal(char * fieldName, double & fieldValue)
		{
			try 
			{
				_variant_t  vtValue;
				vtValue = m_pRecord->Fields->GetItem(fieldName)->GetValue();
				fieldValue = vtValue.dblVal;
				return true;
			}
			catch (_com_error &e) 
			{
				this->errorReport(e);
				SLogPrintAtFile(L"! error query field : %S", fieldName);
			}
			return false;
		}

		bool QueryRecord::GetVal(char * fieldName, long & fieldValue)
		{
			try 
			{
				_variant_t  vtValue;
				vtValue = m_pRecord->Fields->GetItem(fieldName)->GetValue();
				fieldValue = vtValue.lVal;
				return true;
			}
			catch (_com_error &e) 
			{
				this->errorReport(e);
				SLogPrintAtFile(L"! error query field : %S", fieldName);
			}
			return false;
		}
	}
}