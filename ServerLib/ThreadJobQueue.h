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

			queue<T>* m_pWriteQueue;	//input 용 여러 쓰레드에서 입력을 받음
			queue<T>* m_pReadQueue;	//output 용. 한 쓰레드에서 출력을 함

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
			함수명	: push
			설명		: writeQueue_에 t를 넣어줌
			리턴값	: 없음
			매개변수	: const T&
			*****************************************************************************/
			inline void push(const T &t)
			{
				Util::SAFE_LOCK(lock_);
			
				m_pWriteQueue->push(t);
			}

			/****************************************************************************
			함수명	: pop
			설명		: readQueue_의 첫번째 데이터를 pop함
			리턴값	: bool
			매개변수	: T&
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
			함수명	: swap
			설명		: readQueue_와 writeQueue_를 스왑함
			리턴값	: 없음
			매개변수	: 없음
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
			함수명	: swap
			설명		: readQueue_가 비었는지 확인
			리턴값	: bool
			매개변수	: 없음
			*****************************************************************************/
			inline bool isEmpty() const { return m_pReadQueue->empty(); }

			/****************************************************************************
			함수명	: size
			설명		: WRITE_QUEUE + READ_QUEUE의 size를 리턴
			리턴값	: size_t
			매개변수	: 없음
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