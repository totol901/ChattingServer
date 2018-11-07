#include "stdafx.h"
#include "Object.h"

static UINT generateId = 0;

Object::Object()
	:m_id(generateId++)
{
}

Object::~Object()
{
}
