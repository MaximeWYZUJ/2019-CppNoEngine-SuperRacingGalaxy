#pragma once

#include <memory>
#include <vector>

#include "Texture.h"

namespace SmallEngine
{
	class Device;
	class TextureManager
	{
	public:
		Texture const* GetNewTexture(const std::wstring& fileName, Device* device);
		Texture const* GetTexture(const std::wstring& fileName) noexcept;
	private:
		std::vector<std::unique_ptr<Texture>> textures;
	};
}
