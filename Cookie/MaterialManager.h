#pragma once

#include <vector>

#include "Material.h"
#include "ExportMacro.h"
#include <memory>

namespace Cookie
{
	class Device;
	class COOKIE_API MaterialManager
	{
	public:
		Material* GetNewMaterial(std::string name, Texture* texture, Vector4<> ambient = Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector4<> diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector4<> specular = Vector4(1.0f, 1.0f, 1.0f, 1.0f), float puissance = 1.0f, bool transparent = false);
	private:
		std::vector<Material*> materials;
		Material* GetMaterial(const std::string& name) noexcept;
	};
}
