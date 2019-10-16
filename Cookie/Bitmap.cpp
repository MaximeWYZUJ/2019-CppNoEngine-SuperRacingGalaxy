#include "pch.h"
#include "Bitmap.h"

namespace SmallEngine
{
	using namespace std;

	Bitmap::Bitmap()
	{
	}

	Bitmap::Bitmap(Size<int>&& size, std::vector<uint8_t>&& data) noexcept
		: size{ move(size) }, data{ move(data) }
	{
	}

	Size<int> const& Bitmap::GetSize() const noexcept
	{
		return size;
	}

	uint8_t Bitmap::operator[](int index) const noexcept
	{
		return data[index];
	}
}
