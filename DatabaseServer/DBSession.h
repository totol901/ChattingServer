#pragma once

namespace DatabaseServer
{
	class DBSession :
		public ServerEngine::Singleton<DBSession>,
		public DatabaseSystem::Database
	{
	private:

	public:
		DBSession();
		~DBSession();
		DBSession(const DBSession&) = delete;
		DBSession(const DBSession&&) = delete;
		DBSession& operator = (const DBSession&) = delete;
		DBSession& operator = (const DBSession&&) = delete;

		/****************************************************************************
		함수명	: CheckUserInfoQuery
		설명		: ID, PW 쿼리 보내고 맞는지 판단
		*****************************************************************************/
		bool CheckUserInfoQuery(const wstring& ID, const wstring& PW);
		bool CheckUserInfoQuery(string ID, string PW);

		/****************************************************************************
		함수명	: InsertUserInfoQuery
		설명		: ID, PW, nickname 생성 쿼리 보내고 DB 저장 및 중복 여부 판단
		*****************************************************************************/
		bool InsertUserInfoQuery(wstring ID, wstring PW, wstring nickname);

		/****************************************************************************
		함수명	: DeleteUserInfoQuery
		설명		: ID 쿼리 보내서 DB 삭제함
		*****************************************************************************/
		bool DeleteUserInfoQuery(wstring ID);

		/****************************************************************************
		함수명	: FindNickname
		설명		: ID 퀘리 보내서 닉네임 불러옴
		*****************************************************************************/
		wstring FindNickname(wstring ID);

		/****************************************************************************
		함수명	: InsertUserLogQuery
		설명		: ID와 Log를 DB에 저장
		*****************************************************************************/
		bool InsertUserLogQuery(wstring ID, wstring log);
	};
}
