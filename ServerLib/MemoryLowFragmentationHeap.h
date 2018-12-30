#pragma once

namespace ServerEngine
{
	namespace Util
	{
		//LFH 사용(윈도우에서 제공하는 힙 메모리 풀)
		inline void StartMemoryLowFragmentationHeap()
		{
			static bool init = false;
			if (init)
			{
				return;
			}
			init = true;

			UINT HeapFragValue = 2;
			HANDLE hHeaps[100];
			DWORD dwHeapCount = GetProcessHeaps(100, hHeaps);

			//LFH 사용 설정	
			for (DWORD i = 0; i < dwHeapCount; i++)
			{
				HeapSetInformation(hHeaps[i],
					HeapCompatibilityInformation,
					&HeapFragValue,
					sizeof(HeapFragValue));
			}

			printf("* Low-fragmentation Heap setting\n");
		}
	}
}