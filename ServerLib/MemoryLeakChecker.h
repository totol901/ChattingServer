#pragma once

namespace ServerEngine
{
	namespace Util
	{
		//메모리 릭 채킹
		inline void StartMemoryLeakCheck()
		{
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
		}

		inline void BreakOutMemoryAllocCount(const int& num)
		{
			_CrtSetBreakAlloc(num);
		}
	}
}