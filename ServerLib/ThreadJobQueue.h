#pragma once

namespace ServerEngine
{
	namespace System
	{
		template <typename T>
		class ThreadJobQueue
		{
		private:
			enum
			{
				WRITE_QUEUE,
				READ_QUEUE,
				MAX_QUEUE,
			};

			queue<T> m_queue[MAX_QUEUE];

			queue<T>* m_pWriteQueue;	//input �� ���� �����忡�� �Է��� ����
			queue<T>* m_pReadQueue;	//output ��. �� �����忡�� ����� ��

			Util::Lock		        m_lock;

		public:
			ThreadJobQueue(WCHAR* name)
				: m_lock(name),
				m_pWriteQueue(&m_queue[WRITE_QUEUE]),
				m_pReadQueue(&m_queue[READ_QUEUE])
			{
			}
			~ThreadJobQueue()
			{
				m_pReadQueue->empty();
				m_pWriteQueue->empty();
			}

			/****************************************************************************
			�Լ���	: push
			����		: writeQueue_�� t�� �־���
			���ϰ�	: ����
			�Ű�����	: const T&
			*****************************************************************************/
			inline void push(const T &t)
			{
				Util::SAFE_LOCK(lock_);
			
				m_pWriteQueue->push(t);
			}

			/****************************************************************************
			�Լ���	: pop
			����		: readQueue_�� ù��° �����͸� pop��
			���ϰ�	: bool
			�Ű�����	: T&
			*****************************************************************************/
			inline bool pop(_Out_ T &t)
			{
				Util::SAFE_LOCK(lock_);

				size_t size = this->size();

				if (size == 0)
				{
					return false;
				}

				if (m_pReadQueue->empty())
				{
					this->swap();
				}

				t = m_pReadQueue->front();
				m_pReadQueue->pop();

				return true;
			}

			/****************************************************************************
			�Լ���	: swap
			����		: readQueue_�� writeQueue_�� ������
			���ϰ�	: ����
			�Ű�����	: ����
			*****************************************************************************/
			inline void swap()
			{
				Util::SAFE_LOCK(lock_);

				if (m_pWriteQueue == &queue_[WRITE_QUEUE])
				{
					m_pWriteQueue = &queue_[READ_QUEUE];
					m_pReadQueue = &queue_[WRITE_QUEUE];
				}
				else
				{
					m_pWriteQueue = &queue_[WRITE_QUEUE];
					m_pReadQueue = &queue_[READ_QUEUE];
				}
			}

			/****************************************************************************
			�Լ���	: swap
			����		: readQueue_�� ������� Ȯ��
			���ϰ�	: bool
			�Ű�����	: ����
			*****************************************************************************/
			inline bool isEmpty() const { return m_pReadQueue->empty(); }

			/****************************************************************************
			�Լ���	: size
			����		: WRITE_QUEUE + READ_QUEUE�� size�� ����
			���ϰ�	: size_t
			�Ű�����	: ����
			*****************************************************************************/
			inline size_t size()
			{
				Util::SAFE_LOCK(lock_);

				size_t size = (size_t)(m_queue[WRITE_QUEUE].size() + m_queue[READ_QUEUE].size());
				
				return size;
			}
		};
	}
}