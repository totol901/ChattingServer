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
#define ASSERT(x) Assert(x, (#x), (__FILE__), __LINE__)				   				

/****************************************************************************
�Լ���	: Assert
����		: condition�� false�� (conditionStr + fileName + lineNo)�� ���â�� ���
���ϰ�	: ����
�Ű�����	: int, LPCWSTR, LPCWSTR, int
*****************************************************************************/
void Assert(int condition, char* conditionStr, char* fileName, int lineNo)
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
	itoa(lineNo, buf, 10);
	msg += buf;

	//�α׿� ��
	SErrPrintAtFile("! error %s", msg.c_str());
}
