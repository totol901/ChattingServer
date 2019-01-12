#pragma once

namespace ServerEngine
{
	namespace Util
	{
		class LockManager : public Singleton<LockManager>
		{
			friend Singleton;
		private:
			size_t m_IDSeed;

		public:
			LockManager();
			~LockManager();

			/****************************************************************************
			함수명	: searchLockCycle
			설명		: 데드락 걸린 Lock을 찾아 리턴해줌
			리턴값	: Lock*
			매개변수	: Lock*
			*****************************************************************************/
			Lock* SearchLockCycle(Lock *newLock);

			/****************************************************************************
			함수명	: checkDeadLock
			설명		: 데드락 걸린 Lock을 리턴해줌
			리턴값	: Lock*
			매개변수	: Lock*
			*****************************************************************************/
			Lock* CheckDeadLock(Lock *lock);

			/****************************************************************************
			함수명	: generalId
			설명		: 새로운 id를 리턴해줌
			리턴값	: size_t
			매개변수	: 없음
			*****************************************************************************/
			const size_t GeneralId();
		};
	}
}
