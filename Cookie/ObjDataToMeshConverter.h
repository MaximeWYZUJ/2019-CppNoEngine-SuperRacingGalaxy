#pragma once

#include "ExportMacro.h"

#include "Mesh.h"
#include "ObjData.h"
#include "CompilerFlags.h"

namespace Cookie
{
	class COOKIE_API ObjDataToMeshConverter
	{
	public:
		using MeshPtr = Mesh::MeshPtr;
		static MeshPtr Convert(ObjData const& source, std::string const& filePath);
	};

	struct Indices
	{
		Indices() = default;

		Indices(int32_t vertex, int32_t texture, int32_t normal)
			: vertex(vertex), texture(texture), normal(normal)
		{
		}

		int32_t* AsArray()
		{
			return reinterpret_cast<int32_t*>(this);
		}

		int32_t vertex;
		int32_t texture;
		int32_t normal;
	};
}

template<>
struct std::hash<Cookie::Indices>
{
	size_t operator()(Cookie::Indices const& indices) const noexcept
	{
		if constexpr (Cookie::ArchWidth == Cookie::_32bits)
		{
			int32_t res = static_cast<int32_t>(indices.vertex) << 16;
			res |= static_cast<int16_t>(indices.texture) << 8;
			res |= static_cast<int8_t>(indices.normal);
			return res;
		}
		else if constexpr (Cookie::ArchWidth == Cookie::_64bits)
		{
			int64_t res = static_cast<int64_t>(indices.vertex) << 32;
			res |= static_cast<int32_t>(indices.texture) << 16;
			res |= static_cast<int16_t>(indices.normal);
			return res;
		}
	}
};

template<>
struct std::equal_to<Cookie::Indices>
{
	bool operator()(Cookie::Indices const& lhs, Cookie::Indices const& rhs) const noexcept
	{
		return lhs.vertex == rhs.vertex &&
			lhs.texture == rhs.texture &&
			lhs.normal == rhs.normal;
	}
};