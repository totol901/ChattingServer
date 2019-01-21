#pragma once

namespace ServerEngine
{
	namespace DatabaseSystem
	{
		constexpr auto DB_PARAM_SIZE = 8192;

		typedef enum
		{
			QUERY_NOT_RETURN,				//리턴없는 쿼리
			QUERY_WAIT_RETURN,				//DB응답을 꼭 기다린뒤 처리하는 쿼리
			QUERY_CALL_BACK,				//DB응답을 기다리지 않지만 뒷처리는 있는 쿼리
		} QUERY_TYPE;

		class QueryStatement
		{
		private:
			::_CommandPtr				p_mCommand;		//ADO 명령문 제어
			QUERY_TYPE				m_Type;			//쿼리 타입

			wstring					m_query;		//쿼리문
			int						m_paramCount;	//파라미터 갯수

		public:
			QueryStatement();
			~QueryStatement();

			void setQuery(WCHAR *query, QUERY_TYPE type = QUERY_NOT_RETURN);

			const WCHAR *GetQuery();
			const QUERY_TYPE& GetType();

			//------------------ 파라메터 추가 -----------------/
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
