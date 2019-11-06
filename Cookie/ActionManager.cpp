#include "pch.h"

#include <stdexcept>

#include "ActionManager.h"
#include <iostream>
#include "magic_enum.hpp"

namespace Cookie
{
	using namespace std;

	ActionManager::ActionManager(InputManager* inputManager)
		: inputManager(inputManager)
	{
		assert(inputManager);
		
		activeContext = end(contexts);
	}

	void ActionManager::CreateContext(std::string const& contextName, vector<ActionDescriptor>&& actions)
	{
		bool const insertionFailed = !contexts.insert({ contextName, ActionContext(move(actions)) }).second;
		if (insertionFailed)
		{
			throw runtime_error("There is already a context with name " + contextName + "!");
		}

		if (!activeContextName.empty())
		{
			SetActiveContext(contextName);
		}
	}

	void ActionManager::SetActiveContext(std::string const& contextName)
	{
		activeContextName = contextName;
		activeContext = contexts.find(contextName);
		
		actionRunners.clear();
		actionRunners.reserve(activeContext->second.actions.size());
		for(auto& descriptor : activeContext->second.actions)
		{
			actionRunners.emplace_back(&descriptor);
		}
	}
	
	void ActionManager::Update()
	{
		for (auto& runner : actionRunners)
		{
			runner.Update();
		}
		
		vector<InputEvent> const& events = inputManager->GetEvents();

		// Todo: really slow if ActionContext are more complex
		for (auto event : events)
		{
			if (event.type == InputEventType::KeyStateChanged)
			{
				for (auto& runner : actionRunners)
				{
					runner.Run(get<KeyStateChanged>(event.data));
				}
			}
		}
	}
}
