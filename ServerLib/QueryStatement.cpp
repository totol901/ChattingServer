#include "stdafx.h"
#include "QueryStatement.h"

namespace ServerEngine
{
	namespace DatabaseSystem
	{
		QueryStatement::QueryStatement()
			:m_paramCount(0)
		{
		}

		QueryStatement::~QueryStatement()
		{
		}

		void QueryStatement::setQuery(WCHAR * query, QUERY_TYPE type)
		{
			m_query = query;
			m_Type = type;
		}

		const WCHAR * QueryStatement::GetQuery()
		{
			return m_query.c_str();
		}

		const QUERY_TYPE& QueryStatement::GetType()
		{
			return m_Type;
		}

		//------------------ 파라메터 추가 -----------------/
		template<typename T>
		void QueryStatement::addArg(const WCHAR *fmt, T value)
		{
			array<WCHAR, DB_PARAM_SIZE> buffer;
			_snwprintf_s(buffer.data(), buffer.size(), _TRUNCATE, fmt, value);

			if (m_paramCount++)
			{
				m_query += L", ";
			}
			else
			{
				m_query += L" ";
			}

			m_query += buffer.data();
		}

		void QueryStatement::addParam(CHAR * value)
		{
			this->addArg(L"'%S'", value);
		}

		void QueryStatement::addParam(WCHAR * value)
		{
			this->addArg(L"'%s'", value);
		}

		void QueryStatement::addParam(INT32 value)
		{
			this->addArg(L"'%d'", value);
		}

		void QueryStatement::addParam(UINT32 value)
		{
			this->addArg(L"'%u'", value);
		}

		void QueryStatement::addParam(INT64 value)
		{
			this->addArg(L"'%lld'", value);
		}

		void QueryStatement::addParam(UINT64 value)
		{
			this->addArg(L"'%llu'", value);
		}

		void QueryStatement::addParam(FLOAT value)
		{
			this->addArg(L"'%f'", value);
		}

		void QueryStatement::addParam(double value)
		{
			this->addArg(L"'%lf'", value);
		}
	}
}
