#include "pch.h"

#include <string>
#include <utility>

#include "Material.h"
#include "Vector4.h"

namespace Cookie
{
	using namespace std;

	Material::Material(string name, Texture const* texture, Vector4<> const& ambient, Vector4<> const& diffuse, Vector4<> const& specular, float power, bool transparent)
		: matName(move(name))
		, ambient(ambient)
		, diffuse(diffuse)
		, specular(specular)
		, power(power)
		, transparent(transparent)
		, texture(texture)
	{}
}
