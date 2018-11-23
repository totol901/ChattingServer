/******************************************************************************
파일명	: GameResource.h
목적		: 게임에 쓰이는 리소스들 추상화
******************************************************************************/
#pragma once

class GameResource
{
protected:
	wstring m_ResourceName;
	UINT m_ResourceNum;

public:
	GameResource(wstring ResourceName);
	virtual ~GameResource();

	const wstring& GetName() { return m_ResourceName; }
	const UINT& GetResourceNum() { return m_ResourceNum; }
};
