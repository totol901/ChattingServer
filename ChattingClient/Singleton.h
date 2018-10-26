/******************************************************************************
파일명	: Singleton.h
목적		: 싱글턴 패턴 생성
사용방식	: public 상속을 받아 사용
******************************************************************************/
#pragma once

template<class T>
class Singleton
{
private:
	static T* m_pInstatnce;

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
		if (m_pInstatnce == nullptr)
		{
			m_pInstatnce = new T;
		}

		return m_pInstatnce;
	}

	/****************************************************************************
	함수명	: DestroyInstance
	설명		: 인스턴스를 지워줌
	*****************************************************************************/
	void DestroyInstance()
	{
		SAFE_DELETE(m_pInstatnce);
	}
};

//m_pInstatnce를 nullptr로 초기화
template<class T> T* Singleton<T>::m_pInstatnce = nullptr;
