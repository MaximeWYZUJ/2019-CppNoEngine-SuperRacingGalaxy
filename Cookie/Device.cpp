#include "pch.h"
#include "Device.h"

namespace Cookie
{
	uint32_t Device::GetWidth() const
	{
		return screenWidth;
	}

	uint32_t Device::GetHeight() const
	{
		return screenHeight;
	}

	void Device::PostUpdate()
	{
		events.clear();
	}

	std::vector<DeviceEvent<>> const& Device::GetEvents() const
	{
		return events;
	}

	bool Device::HasFocus() const
	{
		return hasFocus;
	}
}
