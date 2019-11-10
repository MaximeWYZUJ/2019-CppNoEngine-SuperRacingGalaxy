#include "pch.h"

#define _USE_MATH_DEFINES

#include "EntryPoint.h"
#include "BitmapToMeshAdapter.h"
#include "SceneManager.h"
#include "MaterialManager.h"
#include <DeviceD3D11.h>
#include "Material.h"
#include "PhysicsBoxComponent.h"
#include "Vector4.h"

using namespace std;
using namespace Cookie;

int main(int argc, char* argv[])
{
	try
	{
		unique_ptr<Engine> engine = EntryPoint::CreateStandaloneEngine();

		Device* device = engine->GetDevice();
		SceneManager* smgr = engine->GetSceneManager();
		InputManager* inputManager = engine->GetInputManager();
		ActionManager* actionManager = engine->GetActionManager();
		
		TextureManager* tm = engine->GetTextureManager();
		MaterialManager* mm = engine->GetMaterialManager();

		Mesh* mesh = smgr->GetMesh("cube.obj");

		SceneNode* root = smgr->GetRoot();
		
		auto texture = tm->GetNewTexture(L"UneTexture.dds", device);

		// Creation du plan
		SceneNode* planeNode = smgr->AddSceneNode(root);
		planeNode->localTransform.SetPosition({ 5.0f, 0.0f, 0.0f });
		planeNode->localTransform.SetScale({ 2.5f, 0.05f, 5.0f });
		planeNode->localTransform.SetRotation(Quaternion<>::FromDirection(M_PI / 6, { 0.0f, 0.0f, 1.0f }));
		
		auto mat = mm->GetNewMaterial("basic", texture, { 0.0f, 0.0f, 0.0f, 1.0f}, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, 4, false);
		smgr->AddMeshRenderer(mesh, mat, planeNode);

		smgr->AddPhysicsBoxComponent(
			{ 5.0f, 0.0f, 0.0f }, Quaternion<>::FromDirection(M_PI / 6, { 0.0f, 0.0f, 1.0f }), PhysicMaterial(0.5f, 0.5f, 0.6f), PhysicsComponent::STATIC, 5.0f, 0.1f, 10.0f,
			planeNode
		);

		// Creation du cube
		SceneNode* cubeNode = smgr->AddSceneNode(root);
		cubeNode->localTransform.SetPosition({ 3.0f, 0.0f, 2.0f });
		auto const mat2 = mm->GetNewMaterial("basic2", texture);
		smgr->AddMeshRenderer(mesh, mat2, cubeNode);
		
		smgr->AddPhysicsBoxComponent(
			{ 3.0f, 10.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, PhysicMaterial(0.5f, 0.5f, 0.6f), PhysicsComponent::DYNAMIC, 2.0f, 2.0f, 2.0f,
			cubeNode
		);
		
		// Creation de la camera
		SceneNode* camNode = smgr->AddSceneNode(root);
		Camera* cam = smgr->AddCamera(camNode);
		smgr->SetMainCamera(cam);
		camNode->localTransform.SetPosition(Vector3<>(0.0f, 5.0f, -10.0f));

		while (engine->Run([&camNode, &inputManager]()
		{
			Transform<>& cam = camNode->localTransform;
			Vector3<> curPos = cam.GetPosition();
			Vector4<> forwardNoRot(0.0f, 0.0f, 1.0f, 1.0f);
			Quaternion curRotation = cam.GetRotation();

			Vector4<> forward = Matrix4x4<>::FromRotation(curRotation) * forwardNoRot;
			Vector4<> left = Vector4<>::CrossProduct(forward, Vector4<>(0.0f, 1.0f, 0.0f, 1.0f));

			if (inputManager->IsKeyPressed(Key::W))
			{
				curPos += forward * 0.1f;
			}
			if (inputManager->IsKeyPressed(Key::A))
			{
				curPos += left * 0.1f;
			}
			if (inputManager->IsKeyPressed(Key::S))
			{
				curPos -= forward * 0.1f;
			}
			if (inputManager->IsKeyPressed(Key::D))
			{
				curPos += -left * 0.1f;
			}

			cam.SetPosition(curPos);
		}));

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