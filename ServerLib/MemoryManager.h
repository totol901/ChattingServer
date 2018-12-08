#pragma once

/*
** MemoryManager **
Allocator�� �����ϴ� �Ŵ��� ��ü
*/
namespace ServerEngine
{
	namespace System
	{
		class MemoryManager : public Singleton<MemoryManager>
		{
			friend Singleton;
		public:
			enum E_Allocator
			{
				E_LinearAllocator,
				E_StackAllocator,
				E_FreeListAllocator,
				E_PoolAllocator
			};

		public:
			/*������Ʈ�� ���� Allocator�� �ʱ�ȭ */
			HRESULT Init();
			void Release();
			/*������Ʈ�� �߰��� Allocator ���� */
			void AddNewAllocator(E_Allocator e, const size_t AllocatorSize, const TCHAR* name, const size_t& classSize = 0);
			/*�ش� ID�� Allocator ����*/
			//void DeleteAllocator(const int AllocaotrID);
			/*�ش� �̸��� Allocator ����*/
			void DeleteAllocator(const TCHAR* name);
			/*��� Allocator�� ����*/
			void DeleteAllAllocators();
			/*����ȭ�� �޸� ���� ����*/
			//void DefragPool();
			//void QuaryPool();

			const unordered_map<wstring, Allocator*>& GetHashTable() { return m_HashMap; }
			Allocator* GetAllocator(const TCHAR * name);
			//Allocator* GetAllocator(const int AllocaotrID);

		private:
			MemoryManager();
			~MemoryManager();
			MemoryManager(const MemoryManager&) = delete;
			MemoryManager(const MemoryManager&&) = delete;
			MemoryManager& operator = (const MemoryManager&) = delete;
			MemoryManager& operator = (const MemoryManager&&) = delete;


			unordered_map<wstring, Allocator*> m_HashMap;

			size_t m_TotalHeapSize;
			size_t m_UsedHeapSize;
			size_t m_AllocatorCountNumber;

			Allocator* m_pFirstAllocator;
			Allocator* m_pLastAllocator;

		};
	}
}
