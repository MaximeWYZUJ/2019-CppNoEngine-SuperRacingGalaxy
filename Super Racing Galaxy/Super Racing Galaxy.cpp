#include "pch.h"

#define _USE_MATH_DEFINES

#include "EntryPoint.h"
#include "BitmapToMeshAdapter.h"
#include "SceneManager.h"
#include "MaterialManager.h"
#include <DeviceD3D11.h>
#include "Material.h"
#include "Vector4.h"
#include "ScenarioLoader.h"
#include "DemoScenario.h"
#include "Vehicle.h"

using namespace std;
using namespace Cookie;

float camRadY = M_PI;
float camRadZX = 0.0f;
float camDistance = 10.0f;

int main(int argc, char* argv[])
{
	try
	{
		unique_ptr<Engine> engine = EntryPoint::CreateStandaloneEngine();

		Device* device = engine->GetDevice();
		SceneManager* smgr = engine->GetSceneManager();
		InputManager* inputManager = engine->GetInputManager();
		TextureManager* tm = engine->GetTextureManager();
		MaterialManager* mm = engine->GetMaterialManager();

		Mesh* mesh = smgr->GetMesh("cube.obj");
		SceneNode* root = smgr->GetRoot();
		auto texture = tm->GetNewTexture(L"UneTexture.dds", device);

		// Create Scenario
		Scenario scenario = ScenarioCreator::CreateDemoScenario();
		ScenarioLoader::LoadScenario(engine.get(), scenario);


		// Creation du cube
		SceneNode* cubeNode = smgr->AddSceneNode(root);
		cubeNode->localTransform.SetPosition({ 0.0f, 1200.0f, 0.0f });
		cubeNode->localTransform.SetScale({ 2.0f, 2.0f, 2.0f });
		auto const mat2 = mm->GetNewMaterial("basic2", texture, { 1.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
		smgr->AddMeshRenderer(mesh, mat2, cubeNode);
		
		PhysicsComponent* boxComponent = smgr->AddPhysicsBoxComponent(
			PhysicMaterial(0.0f, 0.5f, 0.0f),
			PhysicsComponent::DYNAMIC,
			cubeNode
		);
		
		
		// Creation de la camera
		SceneNode* camNode = smgr->AddSceneNode(root);
		Camera* cam = smgr->AddCamera(camNode);
		smgr->SetMainCamera(cam);
		camNode->localTransform.SetPosition(Vector3<>(0.0f, 5.0f, -10.0f));

		while (engine->Run([camNode, cubeNode, inputManager, boxComponent]()
		{
			Vector2<int> mouseDelta = inputManager->GetMouseDelta();
			if (inputManager->IsMouseButtonPressed(MouseButton::LeftMouseButton))
			{
				camRadY -= mouseDelta.x * 0.005f;
				camRadZX -= mouseDelta.y * 0.005f;
			}

			Transform<>& cam = camNode->localTransform;
			Vector4<> initialPosNoRot(0.0f, 0.0f, camDistance, 1.0f);

			// Find Camera position
			Quaternion<> yCamRot = Quaternion<>::FromDirection(camRadY, { 0.0f, 1.0f, 0.0f });
			Vector4<> zxDir = Matrix4x4<>::FromRotation(yCamRot) * initialPosNoRot;
			Vector4<> zxRotAxis = Vector4<>::CrossProduct(zxDir, Vector4<>(0.0f, 1.0f, 0.0f, 1.0f));
			zxRotAxis.Normalize();
			Quaternion zxCamRot = Quaternion<>::FromDirection(camRadZX, zxRotAxis);
			Vector3<> curPos = Matrix4x4<>::FromRotation(yCamRot * zxCamRot) * initialPosNoRot;
			Vector3<> cubeOffset = cubeNode->localTransform.GetPosition();
			curPos += cubeOffset;
			cam.SetPosition(curPos);

			// Find camera rotation
			Quaternion<> rCamRotY = Quaternion<>::FromDirection(camRadY - M_PI, { 0.0f, 1.0f, 0.0f });
			Vector4<> forward = cubeOffset - curPos;
			Vector4<> left = Vector4<>::CrossProduct(forward, Vector4<>(0.0f, 1.0f, 0.0f, 1.0f));
			left.Normalize();
			Quaternion<> rCamRotZX = Quaternion<>::FromDirection(-camRadZX, left);
			cam.SetRotation(rCamRotY * rCamRotZX);

			Vector4<> forwardForceDir = Vector4<>::Normalize(-zxDir);
			Vector4<> leftForceDir = Vector4<>::Normalize(Vector4<>::CrossProduct(forwardForceDir, { 0.0f, 1.0f, 0.0f, 1.0f }));

			if (inputManager->IsKeyPressed(Key::W))
			{
				boxComponent->addForce(forwardForceDir * 15.0f);
			}

			if (inputManager->IsKeyPressed(Key::A))
			{
				boxComponent->addForce(leftForceDir * 10.0f);
			}

			if (inputManager->IsKeyPressed(Key::S))
			{
				boxComponent->addForce(forwardForceDir * -10.0f);
			}

			if (inputManager->IsKeyPressed(Key::D))
			{
				boxComponent->addForce(leftForceDir * -10.0f);
			}
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