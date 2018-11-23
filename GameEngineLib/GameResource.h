/******************************************************************************
���ϸ�	: GameResource.h
����		: ���ӿ� ���̴� ���ҽ��� �߻�ȭ
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
