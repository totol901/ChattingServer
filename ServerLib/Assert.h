#pragma once
/******************************************************************************
파일명	: Assert.h
목적		: ASSERT를 패킹
사용방식	: ASSERT(x) 매크로를 사용
******************************************************************************/
#pragma once

#undef ASSERT
/****************************************************************************
매크로명	: ASSERT
설명		: x의 조건이 flase면 assert처리함
매개변수	: 조건식
*****************************************************************************/
#define ASSERT(x) Assert(x, (#x), (__FILE__), __LINE__)				   				

/****************************************************************************
함수명	: Assert
설명		: condition이 false면 (conditionStr + fileName + lineNo)을 경고창에 띄움
리턴값	: 없음
매개변수	: int, LPCWSTR, LPCWSTR, int
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

	//로그에 씀
	SErrPrintAtFile("! error %s", msg.c_str());
}
