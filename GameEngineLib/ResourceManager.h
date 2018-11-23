/******************************************************************************
���ϸ�	: ResourceManager.h
����		: ���ҽ��� �����ϴ� ��ü
******************************************************************************/
#pragma once

class ImageManager;
class TextManager;
class ResourceManager
{
	friend GameEngine;
private:
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager(const ResourceManager&&) = delete;
	ResourceManager& operator = (const ResourceManager&) = delete;
	ResourceManager& operator = (const ResourceManager&&) = delete;
	ResourceManager();
	~ResourceManager();

	ImageManager* m_pImageManager;
	TextManager* m_pTextManager;

public:
	HRESULT Init();
	void Destroy();

	ImageManager* GetImageManager() { return m_pImageManager; }
	TextManager* GetTextManager() { return m_pTextManager; }
};
