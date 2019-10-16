#pragma once

#include "ExportMacro.h"

#include "Mesh.h"
#include "ObjData.h"

namespace SmallEngine
{
	class COOKIE_API ObjDataToMeshConverter
	{
	public:
		static Mesh Convert(ObjData const& source);
	};
}