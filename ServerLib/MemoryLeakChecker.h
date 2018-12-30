#pragma once

namespace ServerEngine
{
	namespace Util
	{
		//�޸� �� äŷ
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