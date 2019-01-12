#pragma once

namespace ServerEngine
{
	namespace Util
	{
		class GlobalValue : public Singleton<GlobalValue>
		{
			friend Singleton;
		private:
			GlobalValue(const GlobalValue&) = delete;
			GlobalValue(const GlobalValue&&) = delete;
			GlobalValue& operator = (const GlobalValue&) = delete;
			GlobalValue& operator = (const GlobalValue&&) = delete;

			GlobalValue()
				:m_IsShutdown(false)
			{};
			~GlobalValue() {};

		public:
			bool m_IsShutdown;

		};
	}
}