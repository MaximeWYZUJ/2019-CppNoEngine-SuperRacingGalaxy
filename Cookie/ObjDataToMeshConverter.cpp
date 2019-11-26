#include "pch.h"

#include <vector>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include "ObjDataToMeshConverter.h"
#include "Vector2.h"

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
					vertices.emplace_back(source.vertices[comb.vertex]);
					textureCoords.emplace_back(source.textureCoords[comb.texture]);
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

			// .obj are in CCW but our engine is in CW, swap Y <-> Z
			triangles.push_back(IndexedTriangle{ .A = vIndices.x, .B = vIndices.z, .C = vIndices.y });
		}

		return new Mesh{ move(vertices), move(textureCoords), move(normals), move(triangles) };
	}
}
