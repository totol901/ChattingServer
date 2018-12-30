#pragma once

namespace ServerEngine
{
	namespace Util
	{
		//LFH ���(�����쿡�� �����ϴ� �� �޸� Ǯ)
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

			//LFH ��� ����	
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