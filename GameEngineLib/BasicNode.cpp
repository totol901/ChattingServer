#include "stdafx.h"
#include "BasicNode.h"

BasicNode::BasicNode(const WCHAR* nodeName, UINT nodeNum)
	:m_NodeName(nodeName),
	m_NodeNum(nodeNum)
{
}

BasicNode::~BasicNode()
{
}
