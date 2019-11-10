#pragma once
#include <unordered_map>
#include "PhysicsEngine.h"
#include "PhysicsComponent.h"

namespace Cookie {

	class Synchronizer
	{
	private:
		std::unordered_map<PhysicsComponent*, PhysicsEngine::ActorPtr> map;
	};
}
