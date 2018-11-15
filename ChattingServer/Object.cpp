#include "stdafx.h"
#include "Object.h"

static UINT generateId = 0;

Object::Object()
	:m_ObjectID(generateId++)
{
}

Object::~Object()
{
}
