#pragma once

namespace ServerEngine
{
	namespace DatabaseSystem
	{
		constexpr auto DB_PARAM_SIZE = 8192;

		typedef enum
		{
			QUERY_NOT_RETURN,				//���Ͼ��� ����
			QUERY_WAIT_RETURN,				//DB������ �� ��ٸ��� ó���ϴ� ����
			QUERY_CALL_BACK,				//DB������ ��ٸ��� ������ ��ó���� �ִ� ����
		} QUERY_TYPE;

		class QueryStatement
		{
		private:
			::_CommandPtr				p_mCommand;		//ADO ��ɹ� ����
			QUERY_TYPE				m_Type;			//���� Ÿ��

			wstring					m_query;		//������
			int						m_paramCount;	//�Ķ���� ����

		public:
			QueryStatement();
			~QueryStatement();

			void setQuery(WCHAR *query, QUERY_TYPE type = QUERY_NOT_RETURN);

			const WCHAR *GetQuery();
			const QUERY_TYPE& GetType();

			//------------------ �Ķ���� �߰� -----------------/
			template<typename T>
			void addArg(const WCHAR *fmt, T value);

			void addParam(CHAR *value);
			void addParam(WCHAR *value);
			void addParam(INT32 value);
			void addParam(UINT32 value);
			void addParam(INT64 value);
			void addParam(UINT64 value);
			void addParam(FLOAT value);
			void addParam(double value);
		};
	}
}
