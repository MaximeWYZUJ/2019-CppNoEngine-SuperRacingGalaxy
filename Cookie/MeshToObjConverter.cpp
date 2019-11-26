#include "pch.h"

#include <sstream>
#include <vector>
#include "MeshToObjConverter.h"

namespace Cookie
{
	using namespace std;

	std::string MeshToObjConverter::Convert(Mesh const& mesh)
	{
		stringstream ss;

		ss << "o " << "MyMesh\n";
		
		vector<Vector3<>> const& points = mesh.GetVertices();
		for (int i = 0; i < points.size(); ++i)
		{
			ss << "v " << points[i].x << " " << points[i].y << " " << points[i].z << "\n";
		}

		vector<Vector3<>> const& normals = mesh.GetNormals();
		for (int i = 0; i < normals.size(); ++i)
		{
			ss << "vn " << normals[i].x << " " << normals[i].y << " " << normals[i].z << "\n";
		}

		vector<IndexedTriangle> const& triangles = mesh.GetTrianglesDx();
		for (int i = 0; i < triangles.size(); ++i)
		{
			ss << "f " << triangles[i].A + 1 << "//" << triangles[i].A + 1 
			<< " " << triangles[i].C + 1 << "//" << triangles[i].C + 1
			<< " " << triangles[i].B + 1 << "//" << triangles[i].B + 1 << "\n";
		}

		return ss.str();
	}
}
