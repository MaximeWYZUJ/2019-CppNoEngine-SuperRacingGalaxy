#pragma once

#include <string>
#include <unordered_map>

#include "ExportMacro.h"
#include "ActionContext.h"
#include "ObjectPool.h"

namespace Cookie
{
	class COOKIE_API ActionManager
	{
	public:
		ActionManager();

		// Create (allocate) an ActionContext and return a reference to it.
		// Any subsequent call to this function may invalidate previous returned references.
		ActionContext& CreateContext(std::string const& contextName);
		void SetActiveContext(std::string const& contextName);
	private:
		ActionContext* activeContext;
		std::unordered_map<std::string, ActionContext*> contexts;
		ObjectPool<ActionContext> contextPool;
	};
}
