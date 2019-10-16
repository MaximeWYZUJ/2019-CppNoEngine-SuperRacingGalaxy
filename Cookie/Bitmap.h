#pragma once

#include "ExportMacro.h"

#include <vector>
#include "Size.h"

namespace Cookie
{
	class COOKIE_API Bitmap
	{
		using size_type = size_t;
	public:
		Bitmap();
		Bitmap(Size<int>&& size, std::vector<uint8_t>&& data) noexcept;
		Size<int> const& GetSize() const noexcept;
		uint8_t operator [](int index) const noexcept;
	private:
		Size<int> size;
		std::vector<uint8_t> data;
	};
}