#include "pch.h"
#include "TextureManager.h"

namespace Cookie
{
	Texture* TextureManager::GetNewTexture(const std::wstring& fileName, Device* device)
	{
		Texture* pTexture = GetTexture(fileName);
		
		if (!pTexture)
		{
			pTexture = new Texture(fileName, device);
			textures.push_back(pTexture);
		}
		
		assert(pTexture);
		return pTexture;
	}
	
	Texture* TextureManager::GetTexture(const std::wstring& fileName) noexcept
	{
		Texture* pTexture = nullptr;
		
		for (auto& texture : textures)
		{
			if (texture->GetFilename() == fileName)
			{
				pTexture = texture;
				break;
			}
		}
		
		return pTexture;
	}
}