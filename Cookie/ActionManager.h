#pragma once

#include <string>
#include <unordered_map>

#include "ExportMacro.h"
#include "ActionContext.h"
#include "ActionRunner.h"

namespace Cookie
{
	class COOKIE_API ActionManager
	{
	public:
		ActionManager(InputManager* inputManager);

		// Create an ActionContext.
		void CreateContext(std::string const& contextName, std::vector<ActionDescriptor>&& actions);

		// Activate a context that will be used in subsequent frames.
		void EnableContext(std::string const& contextName);

		// Disable a context for subsequent frames.
		void DisableContext(std::string const& contextName);

		void Update();
	private:
		struct Context
		{
			Context(ActionContext&& actionContext)
				: actionContext(std::move(actionContext))
			{
				actionRunners.reserve(actionContext.actions.size());
			}
			
			bool isEnabled = false;
			ActionContext actionContext;
			std::vector<ActionRunner> actionRunners;
		};
		
		using Contexts = std::unordered_map<std::string, Context>;
		
		InputManager* inputManager;
		Contexts contexts;
	};
}
