#include "pch.h"
#include "TextureManager.h"

namespace SmallEngine
{
	Texture const* TextureManager::GetNewTexture(const std::wstring& fileName, Device* device)
	{
		Texture const* pTexture = GetTexture(fileName);
		
		if (!pTexture)
		{
			auto texture = std::make_unique<Texture>(fileName, device);
			pTexture = texture.get();
			textures.push_back(std::move(texture));
		}
		
		assert(pTexture);
		return pTexture;
	}
	
	Texture const* TextureManager::GetTexture(const std::wstring& fileName) noexcept
	{
		Texture* pTexture = nullptr;
		
		for (auto& texture : textures)
		{
			if (texture->GetFilename() == fileName)
			{
				pTexture = texture.get();
				break;
			}
		}
		
		return pTexture;
	}
}