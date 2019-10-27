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

	std::vector<DeviceEvent> const& Device::GetEvents() const
	{
		return events;
	}

	void Device::ClearEvents()
	{
		events.clear();
	}

	bool Device::HasFocus() const
	{
		return hasFocus;
	}
}
