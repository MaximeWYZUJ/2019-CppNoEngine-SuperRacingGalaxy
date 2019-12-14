#include "pch.h"

#include <random>

#include "EntryPoint.h"
#include "ScenarioLoader.h"
#include "Scenario.h"
#include "Vehicle.h"
#include "Prefab.h"
#include "Planet.h"
#include "Scenery.h"
#include "Vehicle.h"
#include "Teleport.h"

using namespace std;
using namespace Cookie;

struct CollisionCallbackShip : public PhysicsCollisionCallback {
	void operator()(PhysicsComponent* selfComponent, PhysicsComponent* otherComponent) override {
		//cout << "collision personnalisée" << endl;
	}
};

struct TriggerTeleport : public PhysicsCollisionCallback {
	void operator()(PhysicsComponent* selfComponent, PhysicsComponent* otherComponent) override {
		Teleport* teleport = reinterpret_cast<Teleport*>(selfComponent->userData);
		if (teleport) {
			if (teleport->mayUse() && teleport->linkedTeleport->mayUse()) {
				otherComponent->sceneNode->localTransform.SetPosition(teleport->linkedTeleport->initialTransform.GetPosition());
				otherComponent->transform.SetPosition(teleport->linkedTeleport->initialTransform.GetPosition());
				otherComponent->isDirty = true;

				teleport->useNow();
				teleport->linkedTeleport->useNow();
			}
		}
		else {
			cout << "trigger : nullptr mdr" << endl;
		}
	}
};

void ScenarioLoader::LoadScenario(Engine* engine, Scenario const& scenario)
{
	SceneManager* smgr = engine->GetSceneManager();
	MaterialManager* materialManager = engine->GetMaterialManager();
	TextureManager* textureManager = engine->GetTextureManager();
	Device* device = engine->GetDevice();
	SceneNode* root = smgr->GetRoot();

	for (auto& elem : scenario.gravityGenerators)
	{
		CreateObject(smgr, materialManager, textureManager, device, root, elem);
		for (auto &scenery : elem->setElements) {
			CreateObject(smgr, materialManager, textureManager, device, root, scenery);
		}
	}

	for (auto &elem : scenario.sceneries)
	{
		CreateObject(smgr, materialManager, textureManager, device, root, elem);
	}

	for (auto& elem : scenario.teleports)
	{
		CreateObject(smgr, materialManager, textureManager, device, root, elem);
	}

	CreateObject(smgr, materialManager, textureManager, device, root, scenario.vehicle);
}

void ScenarioLoader::CreateObject(SceneManager* smgr, MaterialManager* materialManager, TextureManager* textureManager, Device* device, SceneNode* root, Prefab* obj)
{
	obj->mesh = smgr->GetMesh(obj->meshPath_);
	obj->texture = textureManager->GetNewTexture(obj->texturePath_, device);
	obj->root = smgr->AddSceneNode(root);
	obj->root->localTransform = obj->initialTransform;

	switch (obj->type_) {
	case Prefab::Type::PLANET:
		InitPlanetObject(smgr, materialManager, static_cast<Planet*>(obj));
		break;

	case Prefab::Type::VEHICLE:
		InitVehicleObject(smgr, materialManager, static_cast<Vehicle*>(obj));
		break;

	case Prefab::Type::SCENERY:
		InitSceneryObject(smgr, materialManager, static_cast<Scenery*>(obj));
		break;

	case Prefab::Type::TELEPORT:
		InitTeleportObject(smgr, materialManager, static_cast<Teleport*>(obj));
		break;

	case Prefab::Type::NOTHING:
		break;
	}
}

void ScenarioLoader::InitPlanetObject(Cookie::SceneManager* smgr, Cookie::MaterialManager* materialManager, Planet* obj)
{
	random_device dev;
	mt19937 rng(dev());
	uniform_real_distribution<float> dist(0.5f, 1.0f);

	auto mat = materialManager->GetNewMaterial(
		"basic " + to_string(obj->initialTransform.GetPosition().x) + to_string(obj->initialTransform.GetPosition().y) + to_string(obj->initialTransform.GetPosition().z),
		obj->texture,
		{ dist(rng), dist(rng), dist(rng), 1.0f },
		{ dist(rng), dist(rng), dist(rng), 1.0f },
		{ dist(rng), dist(rng), dist(rng), 1.0f });

	smgr->AddMeshRenderer(obj->mesh, mat, obj->root);

	obj->root->physics = smgr->AddPhysicsMeshComponent(PhysicMaterial(0.0f, 0.5f, 0.6f), PhysicsComponent::STATIC, *obj->mesh, obj->root);
	obj->root->physics->userData = obj;

	// Filter group
	obj->root->physics->addFilterGroup(FilterGroup::DEFAULT);
	obj->root->physics->addFilterGroup(FilterGroup::PLANET);

	// Mask
	//obj->root->physics->addFilterMask(FilterGroup::DEFAULT);
}

void ScenarioLoader::InitVehicleObject(Cookie::SceneManager* smgr, Cookie::MaterialManager *materialManager, Vehicle* obj)
{
	auto mat = materialManager->GetNewMaterial(
		"basic " + to_string(obj->initialTransform.GetPosition().x) + to_string(obj->initialTransform.GetPosition().y) + to_string(obj->initialTransform.GetPosition().z),
		obj->texture,
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		 { 1.0f, 1.0f, 1.0f, 1.0f });

	smgr->AddMeshRenderer(obj->mesh, mat, obj->root);

	obj->root->physics = smgr->AddPhysicsBoxComponent(PhysicMaterial(0.0f, 0.5f, 0.6f), PhysicsComponent::DYNAMIC, obj->root);
	obj->root->physics->userData = obj;

	// Filter group
	obj->root->physics->addFilterGroup(FilterGroup::DEFAULT);
	obj->root->physics->addFilterGroup(FilterGroup::VEHICULE);

	// Mask
	obj->root->physics->addFilterMask(FilterGroup::PLANET);

	obj->root->physics->changeCollisionCallback<CollisionCallbackShip>();
}

void ScenarioLoader::InitSceneryObject(Cookie::SceneManager* smgr, Cookie::MaterialManager *materialManager, Scenery* obj)
{
	auto mat = materialManager->GetNewMaterial(
		"basic " + to_string(obj->initialTransform.GetPosition().x) + to_string(obj->initialTransform.GetPosition().y) + to_string(obj->initialTransform.GetPosition().z),
		obj->texture,
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f, 1.0f });

	smgr->AddMeshRenderer(obj->mesh, mat, obj->root);

	obj->root->physics = smgr->AddPhysicsBoxComponent(PhysicMaterial(0.0f, 0.5f, 0.6f), PhysicsComponent::STATIC, obj->root);
	//obj->root->physics = smgr->AddPhysicsMeshComponent(PhysicMaterial(0.0f, 0.5f, 0.6f), PhysicsComponent::STATIC, *obj->mesh, obj->root);
	obj->root->physics->userData = obj;

	// Filter group
	obj->root->physics->addFilterGroup(FilterGroup::DEFAULT);

	// Mask
	obj->root->physics->addFilterMask(FilterGroup::DEFAULT);
}

void ScenarioLoader::InitTeleportObject(Cookie::SceneManager* smgr, Cookie::MaterialManager* materialManager, Teleport* obj)
{
	auto mat = materialManager->GetNewMaterial(
		"basic " + to_string(obj->initialTransform.GetPosition().x) + to_string(obj->initialTransform.GetPosition().y) + to_string(obj->initialTransform.GetPosition().z),
		obj->texture,
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f, 1.0f });

	smgr->AddMeshRenderer(obj->mesh, mat, obj->root);
	
	obj->root->physics = smgr->AddPhysicsBoxComponent(PhysicMaterial(0.0f, 0.0f, 0.0f), PhysicsComponent::STATIC, obj->root, true);
	obj->root->physics->userData = obj;

	// Filter group
	obj->root->physics->addFilterGroup(FilterGroup::DEFAULT);

	// Mask
	obj->root->physics->addFilterMask(FilterGroup::VEHICULE);

	obj->root->physics->changeTriggerCallback<TriggerTeleport>();
}
