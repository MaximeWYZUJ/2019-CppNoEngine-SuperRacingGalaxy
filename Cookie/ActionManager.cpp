#include "pch.h"
#include "ActionManager.h"

namespace Cookie
{
	using namespace std;

	ActionManager::ActionManager()
		: activeContext(nullptr), contextPool(16)
	{
	}

	ActionContext& ActionManager::CreateContext(std::string const& contextName)
	{
		ActionContext& context = contextPool.Rent();
		bool const insertionFailed = !contexts.insert({ contextName, &context }).second;
		if (insertionFailed)
		{
			throw exception("Insertion failed!");
		}

		return context;
	}

	void ActionManager::SetActiveContext(std::string const& contextName)
	{
		activeContext = contexts.at(contextName);
	}
}
