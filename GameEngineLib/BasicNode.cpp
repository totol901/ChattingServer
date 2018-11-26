#include "stdafx.h"
#include "BasicNode.h"

static UINT NodeNum = 0;

BasicNode::BasicNode(const WCHAR* nodeName)
	:m_NodeName(nodeName),
	m_NodeNum(NodeNum)
{
	NodeNum++;
}

BasicNode::~BasicNode()
{
}
