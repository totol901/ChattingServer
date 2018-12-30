/******************************************************************************
파일명	: Assert.h
목적		: ASSERT를 패킹
******************************************************************************/
#pragma once


#undef ASSERT
	/****************************************************************************
	매크로명	: ASSERT
	설명		: x의 조건이 flase면 assert처리함
	매개변수	: 조건식
	*****************************************************************************/
#define ASSERT(x) ServerEngine::Util::UserAssert(x, (#x), (__FILE__), __LINE__)				   				

namespace ServerEngine
{
	namespace Util
	{
		/****************************************************************************
		함수명	: UserAssert
		설명		: condition이 false면 (conditionStr + fileName + lineNo)을 경고창에 띄움
		*****************************************************************************/
		inline void UserAssert(int condition, const char* conditionStr, const char* fileName, int lineNo)
		{
			if (condition)
			{
				return;
			}

			string	msg;
			msg = " Assert: ";
			msg += conditionStr;
			msg += ", file : ";
			msg += fileName;
			msg += ", line : ";

			char buf[16];
			_itoa_s(lineNo, buf, 10);
			msg += buf;

			//로그에 씀
			SErrPrintAtFile("! error %s", msg.c_str());
			//메모리 덤프에 씀
			MiniDump::GetInstance()->execptionFilter(NULL);
		}
	}
}
