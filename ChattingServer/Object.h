/******************************************************************************
파일명	: Object.h
목적		: 오브젝트 클래스
******************************************************************************/
#pragma once

enum E_OBJECT_TYPE
{
	NONE_TYPE,
	OBJECT_CHANNEL
};

class Object
{
protected:
	E_OBJECT_TYPE	m_type;
	UINT			m_id;

public:
	Object();
	virtual ~Object();

	UINT GetID() const { return m_id; }
};

