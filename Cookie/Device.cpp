#include "pch.h"
#include "Device.h"

namespace SmallEngine
{
	uint32_t Device::GetWidth() const
	{
		return screenWidth;
	}

	uint32_t Device::GetHeight() const
	{
		return screenHeight;
	}
}
