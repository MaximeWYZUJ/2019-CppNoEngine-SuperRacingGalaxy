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
			ss << "v " << points[i].X << " " << points[i].Y << " " << points[i].Z << "\n";
		}

		vector<Vector3<>> const& normals = mesh.GetNormals();
		for (int i = 0; i < normals.size(); ++i)
		{
			ss << "vn " << normals[i].X << " " << normals[i].Y << " " << normals[i].Z << "\n";
		}

		vector<IndexedTriangle> const& triangles = mesh.GetTriangles();
		for (int i = 0; i < triangles.size(); ++i)
		{
			ss << "f " << triangles[i].A + 1 << "//" << triangles[i].A + 1 
			<< " " << triangles[i].C + 1 << "//" << triangles[i].C + 1
			<< " " << triangles[i].B + 1 << "//" << triangles[i].B + 1 << "\n";
		}

		return ss.str();
	}
}
