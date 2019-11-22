#include "pch.h"

#include <random>

#include "EntryPoint.h"
#include "ScenarioLoader.h"
#include "Scenario.h"
#include "Vehicle.h"
#include "Prefab.h"
#include "Planet.h"
#include "Scenery.h"

using namespace std;
using namespace Cookie;

void ScenarioLoader::LoadScenario(Engine* engine, Scenario const& scenario)
{
	SceneManager* smgr = engine->GetSceneManager();
	MaterialManager* materialManager = engine->GetMaterialManager();
	TextureManager* textureManager = engine->GetTextureManager();
	Device* device = engine->GetDevice();
	SceneNode* root = smgr->GetRoot();

	for (auto& elem : scenario.objects)
	{
		CreateObject(smgr, materialManager, textureManager, device, root, elem);
	}
}

void ScenarioLoader::CreateObject(SceneManager* smgr, MaterialManager* materialManager, TextureManager* textureManager, Device* device, SceneNode* root, Prefab* obj)
{
	obj->mesh = smgr->GetMesh(obj->meshPath_);
	obj->texture = textureManager->GetNewTexture(obj->texturePath_, device);
	obj->root = smgr->AddSceneNode(root);
	obj->root->localTransform = obj->transform_;

	random_device dev;
	mt19937 rng(dev());
	uniform_real_distribution<float> dist(0.5f, 1.0f);

	auto mat = materialManager->GetNewMaterial(
		"basic " + to_string(obj->transform_.GetPosition().x) + to_string(obj->transform_.GetPosition().y) + to_string(obj->transform_.GetPosition().z),
		obj->texture,
		{ dist(rng), dist(rng), dist(rng), 1.0f },
		{ dist(rng), dist(rng), dist(rng), 1.0f },
		{ dist(rng), dist(rng), dist(rng), 1.0f });

	smgr->AddMeshRenderer(obj->mesh, mat, obj->root);

	switch (obj->type_) {
	case Prefab::Type::PLANET:
		InitPlanetObject(smgr, static_cast<Planet*>(obj));
		break;

	case Prefab::Type::VEHICLE:
		InitVehicleObject(smgr, static_cast<Vehicle*>(obj));
		break;

	case Prefab::Type::SCENERY:
		InitSceneryObject(smgr, static_cast<Scenery*>(obj));
		break;

	case Prefab::Type::NOTHING:
		break;
	}
}

void ScenarioLoader::InitPlanetObject(Cookie::SceneManager* smgr, Planet* obj)
{
	smgr->AddPhysicsMeshComponent(PhysicMaterial(0.5f, 0.5f, 0.6f), PhysicsComponent::STATIC, *obj->mesh, obj->root);
}

void ScenarioLoader::InitVehicleObject(Cookie::SceneManager* smgr, Vehicle* obj)
{
	smgr->AddPhysicsBoxComponent(PhysicMaterial(0.0f, 0.5f, 0.0f), PhysicsComponent::DYNAMIC, obj->root);
}

void ScenarioLoader::InitSceneryObject(Cookie::SceneManager* smgr, Scenery* obj)
{
}
