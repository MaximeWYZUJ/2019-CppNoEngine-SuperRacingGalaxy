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

		// Set the ActionContext that will be used in subsequent frames.
		void SetActiveContext(std::string const& contextName);

		void Update();
	private:
		using Contexts = std::unordered_map<std::string, ActionContext>;
		
		InputManager* inputManager;
		
		Contexts::const_iterator activeContext;
		std::string activeContextName;

		std::vector<ActionRunner> actionRunners;
		
		Contexts contexts;
	};
}
