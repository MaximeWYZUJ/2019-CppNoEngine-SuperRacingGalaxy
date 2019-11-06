#include "pch.h"
#include "ActionDescriptor.h"

namespace Cookie
{
	ActionDescriptor::ActionDescriptor(Key key, StateType state, std::chrono::milliseconds const& delay, Callbacks callbacks)
		: key(key),
		state(state),
		delay(delay),
		callbacks(callbacks)
	{
	}
}
