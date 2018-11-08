#pragma once

enum E_OBJECT_TYPE
{
	NONE_TYPE,
	OBJECT_CHANNEL
};

class Object
{
protected:
	E_OBJECT_TYPE m_type;
	UINT m_id;

public:
	Object();
	virtual ~Object();

	UINT GetID() const { return m_id; }

	//virtual void Update();
};

