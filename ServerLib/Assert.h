/******************************************************************************
���ϸ�	: Assert.h
����		: ASSERT�� ��ŷ
******************************************************************************/
#pragma once


#undef ASSERT
	/****************************************************************************
	��ũ�θ�	: ASSERT
	����		: x�� ������ flase�� assertó����
	�Ű�����	: ���ǽ�
	*****************************************************************************/
#define ASSERT(x) ServerEngine::Util::UserAssert(x, (#x), (__FILE__), __LINE__)				   				

namespace ServerEngine
{
	namespace Util
	{
		/****************************************************************************
		�Լ���	: UserAssert
		����		: condition�� false�� (conditionStr + fileName + lineNo)�� ���â�� ���
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

			//�α׿� ��
			SErrPrintAtFile("! error %s", msg.c_str());
			//�޸� ������ ��
			MiniDump::GetInstance()->execptionFilter(NULL);
		}
	}
}
