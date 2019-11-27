#include "pch.h"
#include "Synchronizer.h"

using namespace std;


void Cookie::Synchronizer::SyncUp()
{
	PhysicsEngine& engine = PhysicsEngine::getInstance();

	for_each(mapPhysics.begin(), mapPhysics.end(), [&engine](pair<PhysicsComponent*, PhysicsEngine::ActorPtr> p) {
		engine.UpdateComponent(p.second, p.first);

		p.first->parent->localTransform = p.first->transform;
	});
}

void Cookie::Synchronizer::SyncDown(std::unique_ptr<SceneManager> const & sceneManager)
{
	PhysicsEngine& engine = PhysicsEngine::getInstance();

	vector<PhysicsComponent*> addedPx = sceneManager->addedPhysicsComponents;

	for (int i = 0; i < addedPx.size(); i++) {
		auto newCompo = addedPx.at(i);

		PhysicsEngine::ActorPtr actor = engine.CreateComponent(newCompo);
		mapPhysics.insert(pair{ newCompo, actor });
	}

	for_each(mapPhysics.begin(), mapPhysics.end(), [&engine](pair<PhysicsComponent*, PhysicsEngine::ActorPtr> p) {
		p.first->transform = p.first->parent->localTransform;
		engine.UpdateActor(p.first, p.second);
	});

	sceneManager->addedPhysicsComponents.clear();
}

