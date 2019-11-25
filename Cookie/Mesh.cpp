#include "pch.h"
#include <vector>
#include "Mesh.h"

namespace Cookie
{
	using namespace std;

	Mesh::Mesh(vector<Vector3<>>&& vertices, std::vector<Vector2<>>&& textureCoords, vector<Vector3<>> normals, vector<IndexedTriangle>&& triangles) noexcept
		: vertices{ move(vertices) }, textureCoords(move(textureCoords)), normals{ move(normals) }, triangles{ move(triangles) }
	{
		verticesPx.clear();
		for (auto v : vertices) {
			verticesPx.emplace_back(-v.x, v.y, v.z);
		}
	}

	std::string const& Mesh::GetFilePath() const noexcept
	{
		return filePath;
	}

	vector<Vector3<>> const& Mesh::GetVertices() const noexcept
	{
		return vertices;
	}

	std::vector<Vector3<>> const& Mesh::GetVerticesPx() const noexcept
	{
		
		return verticesPx;
	}

	std::vector<Vector2<>> const& Mesh::GetTextureCoords() const noexcept
	{
		return textureCoords;
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
