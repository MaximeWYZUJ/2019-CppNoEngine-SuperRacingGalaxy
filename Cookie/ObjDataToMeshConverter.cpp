#include "pch.h"

#include <vector>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include "ObjDataToMeshConverter.h"

namespace Cookie
{
	using namespace std;

	auto ObjDataToMeshConverter::Convert(ObjData const& source) -> MeshPtr
	{
		vector<Vector3<>> vertices;
		vertices.reserve(source.vertices.size());

		vector<Vector3<>> normals;
		normals.reserve(source.normals.size());

		vector<IndexedTriangle> triangles;
		triangles.reserve(source.triangles.size());

		// Mapping between one vertex index and all its normal indices
		vector<unordered_set<int32_t>> vnIndices;
		vnIndices.resize(source.vertices.size());

		// Remapping between current v/n (as in file) and the new v/n indices (one normal per vertex)
		unordered_map<int64_t, int32_t> remap;
		remap.reserve(source.triangles.size() * 3);

		// Scan triangles
		for (auto& e : source.triangles)
		{
			Vector3<int32_t> vIndices = e.vertexIndices;
			Vector3<int32_t> nIndices = e.normalIndices;

			vnIndices[vIndices.x].insert(nIndices.x);
			vnIndices[vIndices.y].insert(nIndices.y);
			vnIndices[vIndices.z].insert(nIndices.z);
		}

		// Remap indices
		for (int64_t i = 0; i < vnIndices.size(); ++i)
		{
			auto& e = vnIndices[i];
			for (auto& n : e)
			{
				auto& oVertex = source.vertices[i];
				auto& oNormal = source.normals[n];
				vertices.emplace_back(Vector3<>(oVertex));
				normals.emplace_back(Vector3<>(oNormal));

				remap.insert({ (i << 32) + n, vertices.size() - 1 });
			}
		}

		// Insert all the data
		for (auto& e : source.triangles)
		{
			Vector3<int32_t> const vIndices = e.vertexIndices;
			Vector3<int32_t> const nIndices = e.normalIndices;

			// .OBJ are in CCW but our engine is in CW, swap Y <-> Z
			triangles.emplace_back(IndexedTriangle{
				.A = remap[(static_cast<int64_t>(vIndices.x) << 32) + nIndices.x],
				.B = remap[(static_cast<int64_t>(vIndices.z) << 32) + nIndices.z],
				.C = remap[(static_cast<int64_t>(vIndices.y) << 32) + nIndices.y]
			});
		}

		return new Mesh{ move(vertices), move(normals), move(triangles) };
	}
}
