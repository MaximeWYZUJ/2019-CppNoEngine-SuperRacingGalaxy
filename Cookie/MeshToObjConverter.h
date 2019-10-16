#pragma once

#include "ExportMacro.h"

#include <string>
#include "Mesh.h"

namespace SmallEngine
{
	class COOKIE_API MeshToObjConverter
	{
	public:
		static std::string Convert(Mesh const& mesh);
	};
}