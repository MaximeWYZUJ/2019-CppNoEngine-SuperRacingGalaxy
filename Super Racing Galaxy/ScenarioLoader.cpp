#include "pch.h"
#include "ScenarioLoader.h"
#include "Scenario.h"
#include "Vehicle.h"
#include "Prefab.h"
#include "Planet.h"
#include "Scenery.h"
#include "Teleport.h"
#include "Skybox.h"
#include "Landing.h"
#include "Goal.h"

using namespace std;
using namespace Cookie;

struct CollisionCallbackShip : public PhysicsCollisionCallback {
	void operator()(PhysicsComponent* selfComponent, PhysicsComponent* otherComponent) override {
		//cout << "collision personnalis�e" << endl;
	}
};

struct TriggerTeleport : public PhysicsCollisionCallback {
	void operator()(PhysicsComponent* selfComponent, PhysicsComponent* otherComponent) override {
		Teleport* teleport = reinterpret_cast<Teleport*>(selfComponent->userData);
		if (teleport) {
			if (teleport->mayUse()) {
				teleport->resetCooldown();
				teleport->isActive = true;
				teleport->objToTeleport = static_cast<Prefab*>(otherComponent->userData);
				teleport->printControlPoints(cout);
			}
		}
		else {
			cout << "trigger teleport : nullptr mdr" << endl;
		}
	}
};

struct TriggerGoal : PhysicsCollisionCallback {
	void operator()(PhysicsComponent* selfComponent, PhysicsComponent* otherComponent) override {
		cout << "trigger goal" << endl;
		Goal* g = reinterpret_cast<Goal*>(selfComponent->userData);
		if (g) {
			g->showEndingScreen();
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
		for (auto& scenery : elem->setElements) {
			CreateObject(smgr, materialManager, textureManager, device, elem->root, scenery);
		}
		for (auto& teleport : elem->teleportElements) {
			CreateObject(smgr, materialManager, textureManager, device, elem->root, teleport);
		}

		if (elem->goal)
			CreateObject(smgr, materialManager, textureManager, device, elem->root, elem->goal);
	}

	for (auto& elem : scenario.sceneries)
	{
		CreateObject(smgr, materialManager, textureManager, device, root, elem);
	}

	CreateObject(smgr, materialManager, textureManager, device, root, scenario.vehicle);

	CreateObject(smgr, materialManager, textureManager, device, root, scenario.skybox);

	// Linkage des teleporteurs avec leur piste d'atterissage, et d�finition des points de contr�le
	for_each(scenario.tpLinks.begin(), scenario.tpLinks.end(), [](TeleportLinksParams params) {
		Cookie::Vector3<> pc1;
		if (params.firstDefinedControlPoint.first) {
			pc1 = params.firstDefinedControlPoint.second;
		}
		else {
			auto direction1 = params.teleport->root->localTransform.GetPosition() - params.teleportPlanet->root->localTransform.GetPosition();
			direction1.Normalize();
			pc1 = params.teleport->root->localTransform.GetPosition() + direction1 * params.teleportPlanet->root->localTransform.GetScale().x;
		}

		Cookie::Vector3<> pc2;
		if (params.lastDefinedControlPoint.first) {
			pc2 = params.lastDefinedControlPoint.second;
		} else {
			auto direction2 = params.landing->root->localTransform.GetPosition() - params.landingPlanet->root->localTransform.GetPosition();
			direction2.Normalize();
			pc2 = params.landing->root->localTransform.GetPosition() + direction2 * params.landingPlanet->root->localTransform.GetScale().x;
		}
		params.teleport->linkTo(params.landing, { pc1, pc2 });
	});
}

void ScenarioLoader::CreateObject(SceneManager* smgr, MaterialManager* materialManager, TextureManager* textureManager, Device* device, SceneNode* root, Prefab* obj)
{
	obj->mesh = smgr->GetMesh(obj->meshPath_);
	if (obj->triggerPath_ != "")
		obj->triggerMesh = smgr->GetMesh(obj->triggerPath_);
	if (obj->hitBoxPath_ != "")
		obj->hitBox = smgr->GetMesh(obj->hitBoxPath_);
	obj->texture = textureManager->GetNewTexture(obj->texturePath_, device);

	// (Begin Hack) Place scenery object in the scene root but as if they were children of root, work only for 1 depth (planet->scenery)
	SceneNode* trueRoot = root;
	Transform<> trueTransform = obj->initialTransform;
	if (root->parent)
	{
		trueRoot = root->parent;
		trueTransform = Transform<>::FromMatrix(Matrix4x4<>::FromTransform(root->localTransform) * Matrix4x4<>::FromTransform(obj->initialTransform));
	}

	obj->root = smgr->AddSceneNode(trueRoot);
	obj->root->localTransform = trueTransform;

	// (End Hack)

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

	case Prefab::Type::SKYBOX:
		InitSkyboxObject(smgr, materialManager, static_cast<Skybox*>(obj));
		break;

	case Prefab::Type::GOAL:
		InitGoalObject(smgr, materialManager, static_cast<Goal*>(obj));
		break;

	case Prefab::Type::NOTHING:
		break;
	}
}

void ScenarioLoader::InitPlanetObject(SceneManager* smgr, MaterialManager* materialManager, Planet* obj)
{
	/*random_device dev;
	mt19937 rng(dev());
	uniform_real_distribution<float> dist(0.5f, 1.0f);*/

	auto mat = materialManager->GetNewMaterial(
		"basic " + to_string(obj->initialTransform.GetPosition().x) + to_string(obj->initialTransform.GetPosition().y) + to_string(obj->initialTransform.GetPosition().z),
		obj->texture,
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		{ 0.8f, 0.8f, 0.8f, 1.0f },
		{ 0.0f, 0.0f, 0.0f, 1.0f });

	smgr->AddMeshRenderer(obj->mesh, mat, obj->root);

	obj->root->physics = smgr->AddPhysicsMeshComponent(PhysicMaterial(0.0f, 0.5f, 0.6f), PhysicsComponent::STATIC, *obj->mesh, obj->root);
	obj->root->physics->userData = obj;

	// Filter group
	obj->root->physics->addFilterGroup(FilterGroup::DEFAULT);
	obj->root->physics->addFilterGroup(FilterGroup::PLANET);

	// Mask
	//obj->root->physics->addFilterMask(FilterGroup::DEFAULT);
}

void ScenarioLoader::InitVehicleObject(SceneManager* smgr, MaterialManager* materialManager, Vehicle* obj)
{
	auto mat = materialManager->GetNewMaterial(
		"basic " + to_string(obj->initialTransform.GetPosition().x) + to_string(obj->initialTransform.GetPosition().y) + to_string(obj->initialTransform.GetPosition().z),
		obj->texture,
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		{ 0.8f, 0.8f, 0.8f, 1.0f },
		{ 0.0f, 0.0f, 0.0f, 1.0f });

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

void ScenarioLoader::InitSceneryObject(SceneManager* smgr, MaterialManager* materialManager, Scenery* obj)
{
	auto mat = materialManager->GetNewMaterial(
		"basic " + to_string(obj->initialTransform.GetPosition().x) + to_string(obj->initialTransform.GetPosition().y) + to_string(obj->initialTransform.GetPosition().z),
		obj->texture,
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		{ 0.8f, 0.8f, 0.8f, 1.0f },
		{ 0.0f, 0.0f, 0.0f, 1.0f });

	smgr->AddMeshRenderer(obj->mesh, mat, obj->root);

	obj->root->physics = smgr->AddPhysicsMeshComponent(PhysicMaterial(0.0f, 0.5f, 0.6f), PhysicsComponent::STATIC, *obj->hitBox, obj->root);
	//obj->root->physics = smgr->AddPhysicsBoxComponent(PhysicMaterial(0.0f, 0.5f, 0.6f), PhysicsComponent::STATIC, obj->root);
	//obj->root->physics = smgr->AddPhysicsMeshComponent(PhysicMaterial(0.0f, 0.5f, 0.6f), PhysicsComponent::STATIC, *obj->mesh, obj->root);
	obj->root->physics->userData = obj;

	// Filter group
	obj->root->physics->addFilterGroup(FilterGroup::DEFAULT);

	// Mask
	obj->root->physics->addFilterMask(FilterGroup::DEFAULT);
}

void ScenarioLoader::InitSkyboxObject(SceneManager* smgr, MaterialManager* materialManager, Skybox* obj)
{
	auto mat = materialManager->GetNewMaterial(
		"basic " + to_string(obj->initialTransform.GetPosition().x) + to_string(obj->initialTransform.GetPosition().y) + to_string(obj->initialTransform.GetPosition().z),
		obj->texture,
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		{ 0.8f, 0.8f, 0.8f, 1.0f },
		{ 0.0f, 0.0f, 0.0f, 1.0f });

	smgr->AddMeshRenderer(obj->mesh, mat, obj->root);
}

void ScenarioLoader::InitTeleportObject(SceneManager* smgr, MaterialManager* materialManager, Teleport* obj)
{
	auto mat = materialManager->GetNewMaterial(
		"basic " + to_string(obj->initialTransform.GetPosition().x) + to_string(obj->initialTransform.GetPosition().y) + to_string(obj->initialTransform.GetPosition().z),
		obj->texture,
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		{ 0.8f, 0.8f, 0.8f, 1.0f },
		{ 0.0f, 0.0f, 0.0f, 1.0f });

	smgr->AddMeshRenderer(obj->mesh, mat, obj->root);

	//obj->root->physics = smgr->AddPhysicsMeshComponent(PhysicMaterial(0.0f, 0.0f, 0.0f), PhysicsComponent::STATIC, *obj->triggerMesh, obj->root, true);
	obj->root->physics = smgr->AddPhysicsBoxComponent(PhysicMaterial(0.0f, 0.0f, 0.0f), PhysicsComponent::STATIC, obj->root, true);
	obj->root->physics->userData = obj;

	// Filter group
	obj->root->physics->addFilterGroup(FilterGroup::DEFAULT);

	// Mask
	obj->root->physics->addFilterMask(FilterGroup::VEHICULE);

	obj->root->physics->changeTriggerCallback<TriggerTeleport>();
}

void ScenarioLoader::InitGoalObject(SceneManager* smgr, MaterialManager* materialManager, Goal* obj)
{
	auto mat = materialManager->GetNewMaterial(
		"basic " + to_string(obj->initialTransform.GetPosition().x) + to_string(obj->initialTransform.GetPosition().y) + to_string(obj->initialTransform.GetPosition().z),
		obj->texture,
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		{ 0.8f, 0.8f, 0.8f, 1.0f },
		{ 0.0f, 0.0f, 0.0f, 1.0f });

	smgr->AddMeshRenderer(obj->mesh, mat, obj->root);

	obj->root->physics = smgr->AddPhysicsMeshComponent(PhysicMaterial(0.0f, 0.0f, 0.0f), PhysicsComponent::STATIC, *obj->hitBox, obj->root, true);
	obj->root->physics->userData = obj;

	// Filter group
	obj->root->physics->addFilterGroup(FilterGroup::DEFAULT);

	// Mask
	obj->root->physics->addFilterMask(FilterGroup::VEHICULE);

	obj->root->physics->changeTriggerCallback<TriggerGoal>();
}
