#pragma once

namespace ServerEngine
{
	namespace System
	{
		typedef function<void(void)> WORK_FUNCTION_VOID_VOID;

		class Work
		{
		private:
			WORK_FUNCTION_VOID_VOID function;
			static BOOL m_AllocatorOn;

		public:
			Work();
			virtual ~Work();

			virtual void DoWork();

			void SetCallbackFunction(WORK_FUNCTION_VOID_VOID func) { function = func; }
			WORK_FUNCTION_VOID_VOID GetCallbackFunc() { return function; }

			static void* operator new(size_t allocSize);
			static void operator delete(void* deletepointer);
		};
	}
}