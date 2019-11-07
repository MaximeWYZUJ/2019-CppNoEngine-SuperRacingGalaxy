#pragma once
#include "ExportMacro.h"
#include <string>
#include "Vector4.h"
#include "Texture.h"

namespace Cookie
{

	class COOKIE_API Material
	{
	public:
		Material(std::string&& name, const Texture* texture, Vector4<>&& ambient, Vector4<>&& diffuse, Vector4<>&& specular, float&& puissance, bool&& transparent);

		~Material() = default;
		
		const std::string matName;

		const Vector4<> ambient;
		const Vector4<> diffuse;
		const Vector4<> specular;
		const float puissance;
		const bool transparent;

		const Texture* texture;
	};
}
