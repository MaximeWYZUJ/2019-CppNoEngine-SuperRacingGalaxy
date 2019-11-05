#pragma once

#include <chrono>

#include "ExportMacro.h"
#include "InputManager.h"

namespace Cookie
{
	enum class StateType
	{
		Pressed,
		Released
	};

	class COOKIE_API ActionDescriptor
	{
	public:
		ActionDescriptor(Key key, StateType state, std::chrono::milliseconds const& stateTime);

	private:
		Key key;
		StateType state;
		std::chrono::milliseconds stateTime;
	};
}
