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
		template <class TIteratable>
		static void Split(TIteratable const& buf, char delimiter, std::vector<std::string_view>& result);
	};

	template <class TIteratable>
	void ObjReader::Split(TIteratable const& buf, char delimiter, std::vector<std::string_view>& result)
	{
		result.clear();
		auto it = begin(buf);
		auto it2 = begin(buf);
		while ((it2 = find(it, end(buf), delimiter)) != end(buf))
		{
			result.push_back(std::string_view{ &(*it), static_cast<size_t>(it2 - it) });
			it = it2 + 1;
		}

		if (it != it2)
		{
			result.push_back(std::string_view{ &(*it), static_cast<size_t>(it2 - it) });
		}
	}
}