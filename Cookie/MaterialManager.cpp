#include "pch.h"

#include "MaterialManager.h"
#include "Material.h"

namespace Cookie
{
	Material* MaterialManager::GetNewMaterial(std::string const& name, Texture* texture, Vector4<> ambient, Vector4<> diffuse, Vector4<> specular, float power, bool transparent)
	{
		Material* pMaterial = GetMaterial(name);

		if (!pMaterial)
		{
			pMaterial = new Material (name, texture, ambient, diffuse, specular, power, transparent);
			materials.push_back(pMaterial);
		}

		assert(pMaterial);
		return pMaterial;
	}

	Material* MaterialManager::GetMaterial(const std::string& name) noexcept
	{
		Material* pMaterial = nullptr;

		for (auto& material : materials)
		{
			if (material->matName == name)
			{
				pMaterial = material;
				break;
			}
		}

		return pMaterial;
	}
}