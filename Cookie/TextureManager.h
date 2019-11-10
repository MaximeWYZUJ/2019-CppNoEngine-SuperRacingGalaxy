#pragma once

#include <memory>
#include <vector>

#include "Texture.h"

#include "ExportMacro.h"

namespace Cookie
{
	class Device;

	class COOKIE_API TextureManager
	{
	public:
		Texture* GetNewTexture(const std::wstring& fileName, Device* device);
		Texture* GetTexture(const std::wstring& fileName) noexcept;
	private:
		std::vector<Texture*> textures;
	};
}
