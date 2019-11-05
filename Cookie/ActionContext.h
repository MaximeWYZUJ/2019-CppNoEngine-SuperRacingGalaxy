#pragma once

#include <unordered_map>
#include "ExportMacro.h"
#include "ActionDescriptor.h"

namespace Cookie
{
	class COOKIE_API ActionContext
	{
	public:
		ActionContext();

	private:
		std::unordered_map<ActionDescriptor, void(*)()> actions;
	};
}
