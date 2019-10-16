#include "pch.h"
#include <vector>
#include "Mesh.h"

namespace SmallEngine
{
	using namespace std;

	Mesh::Mesh(vector<Vector3<>>&& points, vector<Vector3<>> normals, vector<IndexedTriangle>&& triangles) noexcept
		: vertices{ move(points) }, normals{ move(normals) }, triangles{ move(triangles) }
	{
	}

	vector<Vector3<>> const& Mesh::GetVertices() const noexcept
	{
		return vertices;
	}

	std::vector<Vector3<>> const& Mesh::GetNormals() const noexcept
	{
		return normals;
	}

	vector<IndexedTriangle> const& Mesh::GetTriangles() const noexcept
	{
		return triangles;
	}
}
