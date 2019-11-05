#pragma once

#include "ExportMacro.h"
#include <unordered_map>
#include "ActionDescriptor.h"

namespace Cookie
{
	class COOKIE_API ActionContext
	{
	public:
		ActionContext();

	private:
		//std::unordered_map<ActionDescriptor, void(*)()> actions;
	};
}
