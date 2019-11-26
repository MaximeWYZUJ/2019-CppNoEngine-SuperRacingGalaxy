#include "pch.h"
#include <vector>
#include "Mesh.h"
#include "Matrix4x4.h"

namespace Cookie
{
	using namespace std;

	Mesh::Mesh(vector<Vector3<>>&& vertices, std::vector<Vector2<>>&& textureCoords, vector<Vector3<>> normals, vector<IndexedTriangle>&& triangles) noexcept
		: verticesPx{ move(vertices) }, textureCoords(move(textureCoords)), normals{ move(normals) }, trianglesPx{ move(triangles) }
	{
		auto rot = Matrix4x4<>::FromRotation(Quaternion<>::FromDirection(M_PI, { 0.0f, 0.0f, 1.0f }));
		auto rot2 = Matrix4x4<>::FromRotation(Quaternion<>::FromDirection(M_PI / 2.0f, { 1.0f, 0.0f, 0.0f }));

		verticesDx.reserve(verticesPx.size());

		for (auto const& v : verticesPx)
		{
			auto& v1 = verticesDx.emplace_back(v.x, v.y, v.z);
			v1 = rot2 * rot * v1;
		}

		trianglesDx.reserve(trianglesPx.size());

		for (auto const& t : trianglesPx)
		{
			trianglesDx.emplace_back(IndexedTriangle{ .A = t.A, .B = t.C, .C = t.B });
		}
	}

	std::string const& Mesh::GetFilePath() const noexcept
	{
		return filePath;
	}

	vector<Vector3<>> const& Mesh::GetVertices() const noexcept
	{
		return verticesDx;
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

	vector<IndexedTriangle> const& Mesh::GetTrianglesDx() const noexcept
	{
		return trianglesDx;
	}

	vector<IndexedTriangle> const& Mesh::GetTrianglesPx() const noexcept
	{
		return trianglesPx;
	}
}
