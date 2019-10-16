#include "pch.h"

#include <vector>
#include <array>
#include "BitmapToMeshAdapter.h"

namespace SmallEngine
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
						.X = static_cast<float>(x),
						.Y = static_cast<float>(y),
						.Z = static_cast<float>(source[x + y * mapSize.Width] / 10.0f)
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

			Vector3<> dir1{ .X = v1.X - v2.X, .Y = v1.Y - v2.Y, .Z = v1.Z - v2.Z };
			Vector3<> dir2{ .X = v3.X - v2.X, .Y = v3.Y - v2.Y, .Z = v3.Z - v2.Z };

			Vector3<> crossProduct = {
				.X = dir1.Y * dir2.Z - dir1.Z * dir2.Y,
				.Y = dir1.Z * dir2.X - dir1.X * dir2.Z,
				.Z = dir1.X * dir2.Y - dir1.Y * dir2.X };

			double length = sqrt(crossProduct.X * crossProduct.X + crossProduct.Y * crossProduct.Y + crossProduct.Z * crossProduct.Z);
			crossProduct.X /= length;
			crossProduct.Y /= length;
			crossProduct.Z /= length;

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
					avgNormal.X += normal.X;
					avgNormal.Y += normal.Y;
					avgNormal.Z += normal.Z;
				}

				double length = sqrt(avgNormal.X * avgNormal.X + avgNormal.Y * avgNormal.Y + avgNormal.Z * avgNormal.Z);
				avgNormal.X /= length;
				avgNormal.Y /= length;
				avgNormal.Z /= length;

				normals[x + y * mapSize.Height] = avgNormal;
			}
		}

		return Mesh(move(vertices), move(normals), move(triangles));
	}
}
