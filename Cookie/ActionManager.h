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
		ActionManager(InputManager* inputManager);

		// Create (allocate) an ActionContext and return a reference to it.
		// Any subsequent call to this function may invalidate previous returned references.
		ActionContext& CreateContext(std::string const& contextName);

		// Set the ActionContext that will be used on next frame
		void SetActiveContext(std::string const& contextName);
	private:
		InputManager* inputManager;
		
		ActionContext* activeContext;
		std::unordered_map<std::string, ActionContext*> contexts;
		ObjectPool<ActionContext> contextPool;
	};
}
