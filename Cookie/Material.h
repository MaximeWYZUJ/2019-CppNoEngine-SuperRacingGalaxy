#pragma once

#include <string>

#include "ExportMacro.h"
#include "Vector4.h"

namespace Cookie
{
	class Texture;

	class COOKIE_API Material
	{
	public:
		Material(std::string name, Texture const* texture, Vector4<> const& ambient, Vector4<> const& diffuse, Vector4<> const& specular, float power, bool transparent);

		~Material() = default;
		
		const std::string matName;

		const Vector4<> ambient;
		const Vector4<> diffuse;
		const Vector4<> specular;
		const float power;
		const bool transparent;

		const Texture* texture;
	};
}
