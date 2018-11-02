#pragma once
/******************************************************************************
���ϸ�	: Assert.h
����		: ASSERT�� ��ŷ
�����	: ASSERT(x) ��ũ�θ� ���
******************************************************************************/
#pragma once

#undef ASSERT
/****************************************************************************
��ũ�θ�	: ASSERT
����		: x�� ������ flase�� assertó����
�Ű�����	: ���ǽ�
*****************************************************************************/
#define ASSERT(x) UserAssert(x, (#x), (__FILE__), __LINE__)				   				

/****************************************************************************
�Լ���	: UserAssert
����		: condition�� false�� (conditionStr + fileName + lineNo)�� ���â�� ���
���ϰ�	: ����
�Ű�����	: int, LPCWSTR, LPCWSTR, int
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
}
