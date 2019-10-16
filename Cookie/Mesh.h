#pragma once

#include <vector>
#include "Vector3.h"
#include "IndexedTriangle.h"
#include "Component.h"

namespace Cookie
{
	/* Mesh that supports a maximum of std::numeric_limits<int>::max() vertices */
	class Mesh : public Component
	{
	public:
		Mesh(std::vector<Vector3<>>&& vertices, std::vector<Vector3<>> normals, std::vector<IndexedTriangle>&& triangles) noexcept;
		std::vector<Vector3<>> const& GetVertices() const noexcept;
		std::vector<Vector3<>> const& GetNormals() const noexcept;
		std::vector<IndexedTriangle> const& GetTriangles() const noexcept;
	private:
		std::vector<Vector3<>> vertices;
		std::vector<Vector3<>> normals;
		std::vector<IndexedTriangle> triangles;
	};
}