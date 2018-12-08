#pragma once

/*
** MemoryManager **
Allocator를 관리하는 매니저 객체
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
			/*프로젝트에 쓰일 Allocator들 초기화 */
			HRESULT Init();
			void Release();
			/*프로젝트에 추가할 Allocator 생성 */
			void AddNewAllocator(E_Allocator e, const size_t AllocatorSize, const TCHAR* name, const size_t& classSize = 0);
			/*해당 ID의 Allocator 삭제*/
			//void DeleteAllocator(const int AllocaotrID);
			/*해당 이름의 Allocator 삭제*/
			void DeleteAllocator(const TCHAR* name);
			/*모든 Allocator들 삭제*/
			void DeleteAllAllocators();
			/*단편화된 메모리 조각 모음*/
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
