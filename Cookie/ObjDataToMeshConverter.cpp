#include "pch.h"

#include <vector>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include "ObjDataToMeshConverter.h"
#include "Vector2.h"
#include "Quaternion.h"
#include <corecrt_math_defines.h>
#include "Matrix4x4.h"

namespace Cookie
{
	using namespace std;

	auto ObjDataToMeshConverter::Convert(ObjData const& source) -> MeshPtr
	{
		vector<Vector3<>> vertices;
		vertices.reserve(source.vertices.size());

		vector<Vector2<>> textureCoords;
		textureCoords.reserve(source.textureCoords.size());

		vector<Vector3<>> normals;
		normals.reserve(source.normals.size());

		vector<IndexedTriangle> triangles;
		triangles.reserve(source.triangles.size());

		unordered_map<Indices, Indices> remap;
		remap.reserve(source.vertices.size() * 3);

		for (auto& e : source.triangles)
		{
			Vector3<int32_t> vIndices = e.vertexIndices;
			Vector3<int32_t> tIndices = e.textureIndices;
			Vector3<int32_t> nIndices = e.normalIndices;

			for (int i = 0; i < 3; ++i)
			{
				Indices comb(
					*(reinterpret_cast<int32_t*>(&vIndices) + i),
					*(reinterpret_cast<int32_t*>(&tIndices) + i),
					*(reinterpret_cast<int32_t*>(&nIndices) + i));

				Indices remappedComb{};

				if (!remap.contains(comb))
				{
					auto rot = Matrix4x4<>::FromRotation(Quaternion<>::FromDirection(M_PI, { 0.0f, 0.0f, 1.0f }));
					auto rot2 = Matrix4x4<>::FromRotation(Quaternion<>::FromDirection(M_PI / 2.0f, { 1.0f, 0.0f, 0.0f }));
					Vector3<>& v = vertices.emplace_back(source.vertices[comb.vertex]);
					v = rot2 * rot * v;
					
					Vector2<>& t = textureCoords.emplace_back(source.textureCoords[comb.texture]);
					t.y = 1.0f - t.y;
					
					normals.emplace_back(source.normals[comb.normal]);

					remappedComb.vertex = vertices.size() - 1;
					remappedComb.texture = textureCoords.size() - 1;
					remappedComb.normal = normals.size() - 1;

					remap.insert({ comb, remappedComb });
				}
				else
				{
					remappedComb = remap.at(comb);
				}

				*(reinterpret_cast<int32_t*>(&vIndices) + i) = remappedComb.vertex;
			}

			triangles.push_back(IndexedTriangle{ .A = vIndices.x, .B = vIndices.z, .C = vIndices.y });
		}

		return new Mesh{ move(vertices), move(textureCoords), move(normals), move(triangles) };
	}
}
