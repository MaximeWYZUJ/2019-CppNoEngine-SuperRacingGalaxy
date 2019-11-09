#include "pch.h"

#define _USE_MATH_DEFINES

#include <fstream>
#include "EntryPoint.h"
#include "BitmapToMeshAdapter.h"
#include "SceneManager.h"
#include "MaterialManager.h"
#include <DeviceD3D11.h>
#include <variant>
#include "Material.h"
#include "PhysicsBoxComponent.h"

int c = 0;

void onBeginCallback()
{
	std::cout << "Begin " << c << std::endl;
	c++;
}

void onSuccessCallback()
{
	std::cout << "Success " << c << std::endl;
	c++;
}

void onCancelCallback()
{
	std::cout << "Cancel " << c << std::endl;
	c++;
}

int main(int argc, char* argv[])
{
	using namespace std;
	using namespace Cookie;

	try
	{
		unique_ptr<Engine> engine = EntryPoint::CreateStandaloneEngine();

		PhysicsBoxComponent plane({ 5.0f, 0.0f, 0.0f }, Quaternion<>::FromDirection(M_PI / 6, { 0.0f, 0.0f, 1.0f }), PhysicMaterial(0.5f, 0.5f, 0.6f), PhysicsComponent::STATIC, 5.0f, 0.1f, 10.0f);
		PhysicsBoxComponent cube({ 3.0f, 10.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, PhysicMaterial(0.5f, 0.5f, 0.6f), PhysicsComponent::DYNAMIC, 2.0f, 2.0f, 2.0f);

		Device* device = engine->GetDevice();
		SceneManager* smgr = engine->GetSceneManager();
		TextureManager* tm = engine->GetTextureManager();
		MaterialManager* mm = engine->GetMaterialManager();
		ActionManager* actionManager = engine->GetActionManager();

		actionManager->CreateContext(
			"Test",
			{
				ActionDescriptor(
					Key::A,
					StateType::Pressed,
					chrono::milliseconds(500),
					ActionDescriptor::Callbacks(&onBeginCallback, &onSuccessCallback, &onCancelCallback))
			});

		actionManager->SetActiveContext("Test");

		Mesh* mesh = smgr->GetMesh("cube.obj");

		SceneNode* root = smgr->GetRoot();
		
		SceneNode* planeNode = smgr->AddSceneNode(root);
		planeNode->localTransform.SetPosition({ 5.0f, 0.0f, 0.0f });
		planeNode->localTransform.SetScale({ 2.5f, 0.05f, 5.0f });
		planeNode->localTransform.SetRotation(Quaternion<>::FromDirection(M_PI / 6, { 0.0f, 0.0f, 1.0f }));

		auto texture = tm->GetNewTexture(L"UneTexture.dds", device);
		
		auto mat = mm->GetNewMaterial("basic", texture, { 0.0f, 0.0f, 0.0f, 1.0f}, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, 4, false);
		smgr->AddMeshRenderer(mesh, mat, planeNode);
		
		SceneNode* cubeNode = smgr->AddSceneNode(root);
		cubeNode->localTransform.SetPosition({ 3.0f, 0.0f, 2.0f });
		auto const mat2 = mm->GetNewMaterial("basic2", texture);
		smgr->AddMeshRenderer(mesh, mat2, cubeNode);

		// We should set the camera here
		// Bind Input Actions for first "scene" (main menu)
		// Bind lambda on Update Hook for game logic

		while (engine->Run([](){}));

		return (int)1;
	}

	catch (const std::exception & E)
	{
		const int BufferSize = 128;
		wchar_t message[BufferSize];

		size_t numCharacterConverted;
		mbstowcs_s(&numCharacterConverted, message, E.what(), BufferSize - 1);
		::MessageBox(nullptr, message, L"Erreur", MB_ICONWARNING);

		return (int)99;
	}

	catch (int codeErreur)
	{
		wchar_t szErrMsg[MAX_LOADSTRING];

		//::LoadString(hInstance, codeErreur, szErrMsg, MAX_LOADSTRING);
		::MessageBox(nullptr, szErrMsg, L"Erreur", MB_ICONWARNING);

		return (int)99; // POURQUOI 99???
	}
}