#pragma once
#include <unordered_map>
#include <queue>
#include "PhysicsEngine.h"
#include "PhysicsComponent.h"
#include "SceneManager.h"
#include "PhysicCollisionCallback.h"

namespace Cookie {

	class COOKIE_API Synchronizer
	{
	private:
		std::unordered_map<PhysicsComponent *, PhysicsEngine::ActorPtr> mapPhysics;

	public:
		void SyncUp();
		void SyncDown(std::unique_ptr<SceneManager> const & sceneManager);
	};

}
