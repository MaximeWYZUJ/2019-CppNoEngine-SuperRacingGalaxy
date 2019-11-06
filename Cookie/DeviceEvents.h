#pragma once

#include "Vector2.h"
#include <variant>

namespace Cookie
{
	enum class DeviceEventType
	{
		Focus,
		FocusLost,
		MouseMove
	};

	struct MouseMove
	{
		Vector2<int> pos;
	};

	struct DeviceEvent
	{
		using DataType = std::variant<MouseMove>;
		
		DeviceEventType type;
		DataType data;
	};
}
