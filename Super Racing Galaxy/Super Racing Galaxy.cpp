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
#include "Planet.h"

using namespace std;
using namespace Cookie;

float camRadY = M_PI;
float camRadZX = 0.0f;
float camDistance = 15.0f;

int main(int argc, char* argv[])
{
	try
	{
		unique_ptr<Engine> engine = EntryPoint::CreateStandaloneEngine();

		//Device* device = engine->GetDevice();
		SceneManager* smgr = engine->GetSceneManager();
		SceneNode *root = smgr->GetRoot();
		InputManager *inputManager = engine->GetInputManager();

		/*
		TextureManager* tm = engine->GetTextureManager();
		MaterialManager* mm = engine->GetMaterialManager();

		Mesh* mesh = smgr->GetMesh("cube.obj");
		auto texture = tm->GetNewTexture(L"cube.dds", device);
		*/

		// Create Scenario
		Scenario scenario = ScenarioCreator::CreateDemoScenario();
		ScenarioLoader::LoadScenario(engine.get(), scenario);

		// Axis
		/*Mesh* axisMesh = smgr->GetMesh("triangle.obj");
		SceneNode* axisNode = smgr->AddSceneNode(root);
		Material* axisMaterial = mm->GetNewMaterial("triangle", texture, { 1.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
		smgr->AddMeshRenderer(axisMesh, axisMaterial, axisNode);

		smgr->AddPhysicsMeshComponent({ 0.0f, 0.5f, 0.0f }, PhysicsComponent::STATIC, *axisMesh, axisNode);*/
		

		// Creation du cube
		/*SceneNode* cubeNode = smgr->AddSceneNode(root);
		cubeNode->localTransform.SetPosition({ 0.0f, 0.0f, 0.0f });
		cubeNode->localTransform.SetScale({ 0.5f, 0.5f, 0.5f });
		auto const mat2 = mm->GetNewMaterial("basic2", texture, { 1.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
		smgr->AddMeshRenderer(mesh, mat2, cubeNode);
		
		auto* cubeBoxComponent = smgr->AddPhysicsBoxComponent(
			PhysicMaterial(0.0f, 0.5f, 0.0f),
			PhysicsComponent::STATIC,
			cubeNode
		);*/
		
		// Creation de la camera
		SceneNode* camNode = smgr->AddSceneNode(root);
		Camera* cam = smgr->AddCamera(camNode);
		smgr->SetMainCamera(cam);
		camNode->localTransform.SetPosition(Vector3<>(0.0f, 5.0f, -10.0f));

		int skip = 0;
		while (engine->Run([&skip, camNode, inputManager, scenario]()
		{
			if (skip > 5) {
				Vehicle *vehicle = static_cast<Vehicle *>(scenario.vehicle);
				vehicle->transform_ = vehicle->root->localTransform;
				Planet *closestPlanet = nullptr;
				long double distanceMin = (numeric_limits<long double>::max)();
				for (auto &planet : scenario.planets) {
					planet->transform_ = planet->root->localTransform;
					if (long double newDistanceMin = planet->transform_.GetPosition().distance(vehicle->transform_.GetPosition()) - (planet->transform_.GetScale().x / 2); newDistanceMin < distanceMin) {
						distanceMin = newDistanceMin;
						closestPlanet = static_cast<Planet*>(planet);
					}
				}

				for (auto &scenery : scenario.sceneries) {
					scenery->transform_ = scenery->root->localTransform;
				}

				vehicle->gravityApplied = (vehicle->transform_.GetPosition() - closestPlanet->gravityCenter);
				vehicle->gravityApplied.normalize();
				vehicle->gravityApplied = vehicle->gravityApplied * closestPlanet->gravityValue;

				vehicle->root->physics->addForce(vehicle->gravityApplied);
			}
			skip++;
			
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
			Vector3<> vehicleOffset = scenario.vehicle->transform_.GetPosition();
			curPos += vehicleOffset;
			cam.SetPosition(curPos);

			// Find camera rotation
			Quaternion<> rCamRotY = Quaternion<>::FromDirection(camRadY - M_PI, { 0.0f, 1.0f, 0.0f });
			Vector4<> forward = vehicleOffset - curPos;
			Vector4<> left = Vector4<>::CrossProduct(forward, Vector4<>(0.0f, 1.0f, 0.0f, 1.0f));
			left.Normalize();
			Quaternion<> rCamRotZX = Quaternion<>::FromDirection(-camRadZX, left);
			cam.SetRotation(rCamRotY * rCamRotZX);

			Vector4<> forwardForceDir = Vector4<>::Normalize(-zxDir);
			Vector4<> leftForceDir = Vector4<>::Normalize(Vector4<>::CrossProduct(forwardForceDir, { 0.0f, 1.0f, 0.0f, 1.0f }));

			auto rot = Matrix4x4<>::FromRotation(scenario.vehicle->transform_.GetRotation());
			Vector3<> vehicleForward = rot * Vector3<>{ 0.0f, 0.0f, 1.0f };
			Vector3<> rightSource = rot * Vector3<>{ 1.0f, 0.0f, 1.0f };
			Vector4<> vehicleRight = rightSource - vehicleForward;
			Vector4<> vehicleUp = Vector4<>::CrossProduct(vehicleForward, vehicleRight);
			vehicleUp.Normalize();
			
			if (inputManager->IsKeyPressed(Key::W))
			{
				scenario.vehicle->root->physics->addForce(vehicleForward * 100.0f);
			}

			if (inputManager->IsKeyPressed(Key::A))
			{
				auto rot = Quaternion<>::FromDirection(M_PI / 180.0f, vehicleUp);
				scenario.vehicle->root->localTransform.SetRotation(rot * scenario.vehicle->root->localTransform.GetRotation());
			}

			if (inputManager->IsKeyPressed(Key::S))
			{
				scenario.vehicle->root->physics->addForce(-vehicleForward * 100.0f);
			}

			if (inputManager->IsKeyPressed(Key::D))
			{
				auto rot = Quaternion<>::FromDirection(-M_PI / 180.0f, vehicleUp);
				scenario.vehicle->root->localTransform.SetRotation(rot *scenario.vehicle->root->localTransform.GetRotation());
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