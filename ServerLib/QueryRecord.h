#pragma once

namespace ServerEngine
{
	namespace DatabaseSystem
	{
		class QueryRecord
		{
		private:
			::_RecordsetPtr	m_pRecord;
			int				m_resultVal;		//Äõ¸® °á°ú°ª

		public:
			QueryRecord();
			~QueryRecord();

			void errorReport(::_com_error &e);

			void operator = (QueryRecord &lvalue);
			void operator = (QueryRecord &&rvalue);
			void operator = (::_RecordsetPtr &lvalue);
			void operator = (::_RecordsetPtr &&rvalue);
			::_RecordsetPtr &resultRecord();

			bool        opened();
			void        close();
			void        setRecord(const ::_RecordsetPtr& record);
			bool		isEof();

			int			resultVal();
			void		setResultVal(const int& result);

			HRESULT		moveNext();
			HRESULT		movePrevious();
			HRESULT		moveFirst();
			HRESULT		moveLast();

			bool		GetVal(char* fieldName, char* fieldValue);
			bool		GetVal(char* fieldName, wchar_t* fieldValue);
			bool		GetVal(char* fieldName, int32_t& fieldValue);
			bool		GetVal(char* fieldName, int64_t& fieldValue);
			bool		GetVal(char* fieldName, float& fieldValue);
			bool		GetVal(char* fieldName, double& fieldValue);
			bool		GetVal(char* fieldName, long& fieldValue);
		};
	}
}
