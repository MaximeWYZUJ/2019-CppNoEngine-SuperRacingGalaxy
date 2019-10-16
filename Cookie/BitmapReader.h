#pragma once

#include "ExportMacro.h"

#include <string>
#include "Bitmap.h"

namespace SmallEngine
{
	class COOKIE_API BitmapReader
	{
	public:
		/*
			Read the entire file and return a bitmap object
		*/
		static Bitmap Read(std::string const& filePath);
	};
}