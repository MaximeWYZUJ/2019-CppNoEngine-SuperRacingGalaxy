#include "pch.h"

#include <vector>
#include <array>
#include "BitmapToMeshAdapter.h"

namespace Cookie
{
	using namespace std;

	Mesh BitmapToMeshAdapter::Convert(Bitmap const& source)
	{
		Size<int> mapSize = source.GetSize();

		vector<Vector3<>> vertices;
		vertices.reserve(mapSize.Width * mapSize.Height);
		vector<IndexedTriangle> triangles;
		triangles.reserve((mapSize.Width - 1) * (mapSize.Height - 1) * 2);

		for (int y = 0; y < mapSize.Height; ++y)
		{
			for (int x = 0; x < mapSize.Width; ++x)
			{
				vertices.push_back(
					Vector3<>
					{
						static_cast<float>(x),
						static_cast<float>(y),
						static_cast<float>(source[x + y * mapSize.Width] / 10.0f)
					});
			}
		}

		for (int y = 0; y < mapSize.Height - 1; ++y)
		{
			for (int x = 0; x < mapSize.Width - 1; ++x)
			{
				int bottomLeft = x + y * mapSize.Width;

				triangles.emplace_back(
					IndexedTriangle{
						.A = static_cast<int32_t>(bottomLeft),
						.B = static_cast<int32_t>(bottomLeft + mapSize.Height + 1),
						.C = static_cast<int32_t>(bottomLeft + 1) });

				triangles.emplace_back(
					IndexedTriangle{
						.A = static_cast<int32_t>(bottomLeft),
						.B = static_cast<int32_t>(bottomLeft + mapSize.Height),
						.C = static_cast<int32_t>(bottomLeft + mapSize.Height + 1) });
			}
		}

		vector<Vector3<>> triangleNormals;
		triangleNormals.reserve(triangles.size());
		for (int i = 0; i < triangles.size(); ++i)
		{
			auto& v1 = vertices[triangles[i].A];
			auto& v2 = vertices[triangles[i].B];
			auto& v3 = vertices[triangles[i].C];

			Vector3<> dir1(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
			Vector3<> dir2(v3.x - v2.x, v3.y - v2.y, v3.z - v2.z);

			Vector3<> crossProduct = {
				dir1.y * dir2.z - dir1.z * dir2.y,
				dir1.z * dir2.x - dir1.x * dir2.z,
				dir1.x * dir2.y - dir1.y * dir2.x };

			double length = sqrt(crossProduct.x * crossProduct.x + crossProduct.y * crossProduct.y + crossProduct.z * crossProduct.z);
			crossProduct.x /= length;
			crossProduct.y /= length;
			crossProduct.z /= length;

			triangleNormals.push_back(crossProduct);
		}

		vector<Vector3<>> normals;
		normals.resize(vertices.size());

		vector<Vector3<>> neighborNormals;
		neighborNormals.reserve(6);

		for (int y = 0; y < mapSize.Height; ++y)
		{
			for (int x = 0; x < mapSize.Width; ++x)
			{
				neighborNormals.clear();

				array<bool, 6> neighbors = { false, false, false, false, false, false };
				array<int, 6> neighborOffsets = { 0, 1, -1, -(mapSize.Width - 1) * 2 - 1, -(mapSize.Width - 1) * 2 - 2, -(mapSize.Width - 1) * 2 + 1 };

				neighbors[0] = neighbors[1] = x < mapSize.Width - 1 && y < mapSize.Height - 1;
				neighbors[2] = x > 0 && y < mapSize.Height - 1;
				neighbors[3] = neighbors[4] = x > 0 && y > 0;
				neighbors[5] = x < mapSize.Width - 1 && y > 0;

				int rightTriOffset = (x + y * (mapSize.Height - 1)) * 2;
				for (int i = 0; i < 6; ++i)
				{
					if (neighbors[i])
					{
						neighborNormals.push_back(triangleNormals[rightTriOffset + neighborOffsets[i]]);
					}
				}

				Vector3<> avgNormal{};
				for (auto& normal : neighborNormals)
				{
					avgNormal.x += normal.x;
					avgNormal.y += normal.y;
					avgNormal.z += normal.z;
				}

				double length = sqrt(avgNormal.x * avgNormal.x + avgNormal.y * avgNormal.y + avgNormal.z * avgNormal.z);
				avgNormal.x /= length;
				avgNormal.y /= length;
				avgNormal.z /= length;

				normals[x + y * mapSize.Height] = avgNormal;
			}
		}

		return Mesh(move(vertices), move(normals), move(triangles));
	}
}
