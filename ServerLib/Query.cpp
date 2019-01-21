#include "stdafx.h"
#include "Query.h"

namespace ServerEngine
{
	namespace DatabaseSystem
	{
		Query::Query()
		{
			m_pStatement = new QueryStatement();
		}

		Query::~Query()
		{
			m_Record.close();
			SAFE_DELETE(m_pStatement);
		}

		void Query::setResult(::_RecordsetPtr record)
		{
			m_Record.setRecord(record);
		}

		QueryRecord & Query::GetResult()
		{
			return m_Record;
		}

		void Query::setStatement(QueryStatement * statement)
		{
			m_pStatement = statement;
		}

		QueryStatement * Query::GetStatement()
		{
			return m_pStatement;
		}
	}
}
