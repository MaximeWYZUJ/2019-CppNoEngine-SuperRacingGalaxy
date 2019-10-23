#pragma once

#include <string>
#include "ExportMacro.h"
#include "ObjData.h"

namespace Cookie
{
	class COOKIE_API ObjReader
	{
	public:
		static ObjData Read(std::string const& filePath);
	private:
		static void Split(std::string_view buf, char delimiter, std::vector<std::string_view>& result);
	};
}