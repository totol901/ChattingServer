#pragma once

namespace ServerEngine
{
	namespace DatabaseSystem
	{
		class Query
		{
		protected:
			QueryStatement* m_pStatement;
			QueryRecord m_Record;

		public:
			Query();
			virtual ~Query();

			void setResult(::_RecordsetPtr record);
			QueryRecord &GetResult();

			void setStatement(QueryStatement *statement);
			QueryStatement *GetStatement();
		};
	}
}
