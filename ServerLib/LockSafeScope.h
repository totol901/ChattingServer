#pragma once

namespace ServerEngine
{
	namespace Util
	{
		class LockSafeScope
		{
		private:
			Lock* m_pLock;

		public:
			LockSafeScope(Lock* lock, LPCWCHAR fileName, int lineNo);
			~LockSafeScope();
		};
	}
}
