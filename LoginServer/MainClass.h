#pragma once

namespace LoginServer
{
	class MainClass
	{
	private:
		bool m_isOn;

	public:
		MainClass();
		~MainClass();


		bool IsOn() { return m_isOn; }

		BOOL Init();
	};

}