/******************************************************************************
颇老疙	: ResourceManager.h
格利		: 府家胶甫 包府窍绰 按眉
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
