/******************************************************************************
파일명	: Singleton.h
목적		: 싱글턴 패턴 생성
사용방식	: public 상속을 받아 사용
******************************************************************************/
#pragma once

template<class T>
class Singleton
{
protected:
	Singleton(const Singleton&) = delete;
	Singleton(const Singleton&&) = delete;
	Singleton& operator = (const Singleton&) = delete;
	Singleton& operator = (const Singleton&&) = delete;

	Singleton() {}
	virtual ~Singleton() {}
	
public:
	/****************************************************************************
	함수명	: GetInstance
	설명		: T 클래스의 인스턴스 주소를 리턴받음
	*****************************************************************************/
	static T* GetInstance()
	{
		static T Instatnce;

		return &Instatnce;
	}
};

