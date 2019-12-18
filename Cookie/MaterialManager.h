#pragma once

#include <string>
#include <vector>

#include "ExportMacro.h"
#include "Vector4.h"

namespace Cookie
{
	class Device;
	class Material;
	class Texture;

	class COOKIE_API MaterialManager
	{
	public:
		Material* GetNewMaterial(std::string const& name, std::vector<Texture const*> textures, Vector4<> ambient = Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector4<> diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector4<> specular = Vector4(1.0f, 1.0f, 1.0f, 1.0f), float power = 1.0f, bool transparent = false);
	private:
		std::vector<Material*> materials;
		Material* GetMaterial(const std::string& name) noexcept;
	};
}
