#include "pch.h"
#include "ActionDescriptor.h"

namespace Cookie
{
	ActionDescriptor::ActionDescriptor(Key key, StateType state, std::chrono::milliseconds const& delay, std::chrono::milliseconds const& repeatDelay, Callbacks callbacks)
		: key(key),
		state(state),
		delay(delay),
		repeatDelay(repeatDelay),
		callbacks(callbacks)
	{
	}
}
