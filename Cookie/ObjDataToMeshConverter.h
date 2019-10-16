#pragma once

#include "ExportMacro.h"

#include "Mesh.h"
#include "ObjData.h"

namespace Cookie
{
	class COOKIE_API ObjDataToMeshConverter
	{
	public:
		using MeshPtr = Mesh::MeshPtr;
		static MeshPtr Convert(ObjData const& source);
	};
}