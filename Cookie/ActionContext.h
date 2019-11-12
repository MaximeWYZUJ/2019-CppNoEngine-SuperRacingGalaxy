#pragma once

#include <vector>

#include "ExportMacro.h"
#include "ActionDescriptor.h"

namespace Cookie
{
	class COOKIE_API ActionContext
	{
	public:
		ActionContext(std::vector<ActionDescriptor>&& actions);

		std::vector<ActionDescriptor> actions;
	};
}
