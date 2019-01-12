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
			virtual void Tick() {};// = 0;
			virtual void WakeUp() {};
			virtual void Suspend() {};
			virtual void Stop() {};
			virtual void Start() {};

			void SetCallbackFunction(WORK_FUNCTION_VOID_VOID func) { function = func; }
			WORK_FUNCTION_VOID_VOID GetCallbackFunc() { return function; }

			static void* operator new(size_t allocSize);
			static void operator delete(void* deletepointer);
		};
	}
}
