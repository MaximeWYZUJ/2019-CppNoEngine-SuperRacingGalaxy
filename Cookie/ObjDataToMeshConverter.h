#pragma once

#include "ExportMacro.h"

#include "Mesh.h"
#include "ObjData.h"

namespace Cookie
{
	class COOKIE_API ObjDataToMeshConverter
	{
	public:
		static Mesh Convert(ObjData const& source);
	};
}