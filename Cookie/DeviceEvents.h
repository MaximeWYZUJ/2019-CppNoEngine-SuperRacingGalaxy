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
		MouseButton,
		MouseWheel
	};

	enum class MouseButtonEventType
	{
		LeftButtonUp,
		LeftButtonDown,
		RightButtonUp,
		RightButtonDown
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

	struct MouseWheelData
	{
		MouseWheelData(float rotation)
			: rotation(rotation)
		{
		}

		float rotation;
	};

	struct DeviceEvent
	{
		using DataType = std::variant<MouseMoveData, MouseButtonData, MouseWheelData>;

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
