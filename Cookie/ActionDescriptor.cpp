#include "pch.h"
#include "ActionDescriptor.h"

namespace Cookie
{
	ActionDescriptor::ActionDescriptor(Key key, StateType state, std::chrono::milliseconds const& stateTime)
		: key(key),
		state(state),
		stateTime(stateTime)
	{
	}
}
