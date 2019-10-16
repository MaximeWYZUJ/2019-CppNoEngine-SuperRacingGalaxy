#pragma once

#include <vector>
#include "Vector4.h"
#include "Vector3.h"

namespace Cookie
{
	struct ObjTriangle
	{
		Vector3<int32_t> vertexIndices;
		Vector3<int32_t> textureIndices;
		Vector3<int32_t> normalIndices;
	};

	struct ObjData
	{
		std::vector<Vector4<>> vertices;
		std::vector<Vector3<>> uvs;
		std::vector<Vector3<>> normals;
		std::vector<ObjTriangle> triangles;
	};
}