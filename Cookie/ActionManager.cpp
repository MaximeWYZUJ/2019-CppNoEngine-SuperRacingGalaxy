#include "pch.h"
#include <stdexcept>
#include "ActionManager.h"
#include "magic_enum.hpp"

namespace Cookie
{
	using namespace std;

	ActionManager::ActionManager(InputManager* inputManager)
		: inputManager(inputManager)
	{
		assert(inputManager);
	}

	void ActionManager::CreateContext(std::string const& contextName, vector<ActionDescriptor>&& actions)
	{
		bool const insertionFailed = !contexts.insert({ contextName, Context(ActionContext(move(actions))) }).second;
		if (insertionFailed)
		{
			throw runtime_error("There is already a context with name " + contextName + "!");
		}
	}

	void ActionManager::EnableContext(std::string const& contextName)
	{
		auto const contextIt = contexts.find(contextName);

		if (contextIt != contexts.end() &&
			!contextIt->second.isEnabled)
		{
			auto& context = contextIt->second;

			context.isEnabled = true;
			context.actionRunners.clear();
			for (auto& descriptor : context.actionContext.actions)
			{
				context.actionRunners.emplace_back(&descriptor);
			}
		}
	}
	
	void ActionManager::DisableContext(std::string const& contextName)
	{
		auto const contextIt = contexts.find(contextName);

		if (contextIt != end(contexts))
		{
			contextIt->second.isEnabled = false;
		}
	}

	auto ActionManager::GetState() const -> StateType
	{
		StateType state;

		for (auto& e : contexts)
		{
			if (e.second.isEnabled)
			{
				state.push_back(e.first);
			}
		}

		return state;
	}

	void ActionManager::SetState(StateType state)
	{
		DisableAllContexts();
		for (auto& name : state)
		{
			auto it = contexts.find(name);
			if (it != end(contexts))
			{
				it->second.isEnabled = true;
			}
		}
	}

	void ActionManager::DisableAllContexts()
	{
		for (auto& c : contexts)
		{
			c.second.isEnabled = false;
		}
	}

	void ActionManager::Update()
	{
		for (auto& [contextName, context] : contexts)
		{
			if (context.isEnabled)
			{
				for (auto& runner : context.actionRunners)
				{
					runner.Update();
				}

				vector<InputEvent> const& events = inputManager->GetEvents();
				cout << events.size() << endl;

				// Todo: really slow if ActionContext are more complex
				for (auto event : events)
				{
					if (event.type == InputEventType::KeyStateChanged)
					{
						for (auto& runner : context.actionRunners)
						{
							runner.Run(get<KeyStateChanged>(event.data));
						}
					}
				}
			}
		}
	}
}
