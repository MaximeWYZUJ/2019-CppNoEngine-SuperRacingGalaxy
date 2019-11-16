#include "pch.h"

#include <random>

#include "EntryPoint.h"
#include "ScenarioLoader.h"
#include "Scenario.h"

using namespace std;
using namespace Cookie;

void ScenarioLoader::LoadScenario(Engine* engine, Scenario const& scenario)
{
	SceneManager* smgr = engine->GetSceneManager();
	MaterialManager* materialManager = engine->GetMaterialManager();
	TextureManager* textureManager = engine->GetTextureManager();
	Device* device = engine->GetDevice();
	Mesh* mesh = smgr->GetMesh("planete.obj");

	SceneNode* root = smgr->GetRoot();
	auto texture = textureManager->GetNewTexture(L"UneTexture.dds", device);

	random_device dev;
	mt19937 rng(dev());
	uniform_real_distribution<float> dist(0.5f, 1.0f);

	for (auto& elem : scenario.objects)
	{
		SceneNode* node = smgr->AddSceneNode(root);
		node->localTransform = elem;

		auto mat = materialManager->GetNewMaterial(
			"basic " + to_string(elem.GetPosition().x) + to_string(elem.GetPosition().y) + to_string(elem.GetPosition().z),
			texture,
			{ dist(rng), dist(rng), dist(rng), 1.0f },
			{ dist(rng), dist(rng), dist(rng), 1.0f },
			{ dist(rng), dist(rng), dist(rng), 1.0f });

		smgr->AddMeshRenderer(mesh, mat, node);

		smgr->AddPhysicsBoxComponent(
			node->localTransform.GetPosition(),
			node->localTransform.GetRotation(),
			PhysicMaterial(0.5f, 0.5f, 0.6f),
			PhysicsComponent::STATIC,
			//node->localTransform.GetScale() * 2.0f,
			{1000.f, 1000.f, 1000.f},
			node);
	}
}
