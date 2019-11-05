#pragma once

#include "Vector2.h"

namespace Cookie
{
	enum class DeviceEventType
	{
		Focus,
		FocusLost,
		MouseMove
	};

	template<class T = void>
	struct DeviceEvent
	{
		DeviceEventType type;
		T* data;

		template<class U>
		DeviceEvent<U> const& As()
		{
			return *reinterpret_cast<DeviceEvent<U>*>(this);
		}
	};

	struct MouseMove
	{
		Vector2<int> pos;
	};
}
