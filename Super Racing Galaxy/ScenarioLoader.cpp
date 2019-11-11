#include "pch.h"

#include "EntryPoint.h"
#include "ScenarioLoader.h"
#include "Scenario.h"

using namespace Cookie;

void ScenarioLoader::LoadScenario(Engine* engine, Scenario const& scenario)
{
	SceneManager* smgr = engine->GetSceneManager();
	MaterialManager* materialManager = engine->GetMaterialManager();
	TextureManager* textureManager = engine->GetTextureManager();
	Device* device = engine->GetDevice();
	Mesh* mesh = smgr->GetMesh("cube.obj");

	SceneNode* root = smgr->GetRoot();
	auto texture = textureManager->GetNewTexture(L"UneTexture.dds", device);

	for (auto& elem : scenario.objects)
	{
		SceneNode* node = smgr->AddSceneNode(root);
		node->localTransform = elem;

		auto mat = materialManager->GetNewMaterial("basic", texture);
		smgr->AddMeshRenderer(mesh, mat, node);

		smgr->AddPhysicsBoxComponent(
			node->localTransform.GetPosition(),
			node->localTransform.GetRotation(),
			PhysicMaterial(0.5f, 0.5f, 0.6f),
			PhysicsComponent::STATIC,
			node->localTransform.GetScale() * 2.0f,
			node);
	}
}
