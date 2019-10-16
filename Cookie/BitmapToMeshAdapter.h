#pragma once

#include "ExportMacro.h"

#include "Mesh.h"
#include "Bitmap.h"

namespace Cookie
{
	class COOKIE_API BitmapToMeshAdapter
	{
	public:
		static Mesh Convert(Bitmap const& source);
	};
}