#include "pch.h"
#include "MaterialManager.h"

namespace Cookie
{
	Material* MaterialManager::GetNewMaterial(std::string name, Texture* texture, Vector4<> ambient, Vector4<> diffuse, Vector4<> specular, float puissance, bool transparent)
	{
		Material* pMaterial = GetMaterial(name);

		if (!pMaterial)
		{
			pMaterial = new Material (std::move(name), texture, std::move(ambient), std::move(diffuse), std::move(specular), std::move(puissance), std::move(transparent));
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