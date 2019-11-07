#include "pch.h"

#include "ActionContext.h"

namespace Cookie
{
	using namespace std;
	
	ActionContext::ActionContext(vector<ActionDescriptor>&& actions)
		: actions(move(actions))
	{
	}
}
