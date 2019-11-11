#pragma once

#include "Vector2.h"
#include <variant>

namespace Cookie
{
	enum class DeviceEventType
	{
		Focus,
		FocusLost,
		MouseMove,
		MouseButton
	};

	enum class MouseButtonEventType
	{
		LeftButtonUp,
		LeftButtonDown
	};

	struct MouseMoveData
	{
		Vector2<int> pos;
	};

	struct MouseButtonData
	{
		MouseButtonData(MouseButtonEventType data, Vector2<int> pos)
			: data(data), pos(pos)
		{
		}

		MouseButtonEventType data;
		
		Vector2<int> pos;
	};

	struct DeviceEvent
	{
		using DataType = std::variant<MouseMoveData, MouseButtonData>;

		DeviceEvent(DeviceEventType type)
			: DeviceEvent(type, MouseMoveData())
		{
		}

		DeviceEvent(DeviceEventType type, DataType data)
			: type(type), data(data)
		{
		}
		
		DeviceEventType type;
		DataType data;
	};
}
