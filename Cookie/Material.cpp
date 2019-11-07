#include "pch.h"
#include "Material.h"
#include "DeviceD3D11.h"
#include "util.h"
#include "resource.h"
#include "ShaderParams.h"
#include "SommetBloc.h"
#include "Vector4.h"
#include <string>

namespace Cookie
{
	using namespace std;
	using namespace DirectX;

	Material::Material(std::string&& name, const Texture* texture, Vector4<>&& ambient, Vector4<>&& diffuse, Vector4<>&& specular, float&& puissance, bool&& transparent)
		: matName(move(name))
		, ambient(move(ambient))
		, diffuse(move(diffuse))
		, specular(move(specular))
		, puissance(move(puissance))
		, transparent(move(transparent))
		, texture(texture)
	{}
}
