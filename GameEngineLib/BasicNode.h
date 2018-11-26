#pragma once

class BasicNode
{
private:
	wstring m_NodeName;
	UINT m_NodeNum;

public:
	BasicNode(const WCHAR* nodeName);
	virtual ~BasicNode();

	inline void SetNodeName(const wstring& nodeName) { m_NodeName = nodeName; }
	inline const wstring& GetNodeName() const { return m_NodeName; }
	inline void SetNodeNum(const UINT& nodeNum) { m_NodeNum = nodeNum; }
	inline const UINT& GetNodeNum() const { return m_NodeNum; }
};
