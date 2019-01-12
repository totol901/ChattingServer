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
			�Լ���	: searchLockCycle
			����		: ����� �ɸ� Lock�� ã�� ��������
			���ϰ�	: Lock*
			�Ű�����	: Lock*
			*****************************************************************************/
			Lock* SearchLockCycle(Lock *newLock);

			/****************************************************************************
			�Լ���	: checkDeadLock
			����		: ����� �ɸ� Lock�� ��������
			���ϰ�	: Lock*
			�Ű�����	: Lock*
			*****************************************************************************/
			Lock* CheckDeadLock(Lock *lock);

			/****************************************************************************
			�Լ���	: generalId
			����		: ���ο� id�� ��������
			���ϰ�	: size_t
			�Ű�����	: ����
			*****************************************************************************/
			const size_t GeneralId();
		};
	}
}
