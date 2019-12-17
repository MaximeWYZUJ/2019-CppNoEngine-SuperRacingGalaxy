#include "pch.h"

#include "EntryPoint.h"
#include "SceneManager.h"
#include <DeviceD3D11.h>
#include "Vector4.h"
#include "ScenarioLoader.h"
#include "DemoScenario.h"
#include "Vehicle.h"
#include "Planet.h"
#include "Goal.h"
#include "Teleport.h"
#include "CameraLogic.h"
#include "HUDLogic.h"
#include "VehicleHovering.h"
#include "Vector3.h"
#include "Text.h"

#undef max

using namespace std;
using namespace Cookie;
using namespace Srg;

float camDistance = 8.0f;

int main(int argc, char* argv[])
{
	try
	{
		unique_ptr<Engine> engine = EntryPoint::CreateStandaloneEngine();

		SceneManager* smgr = engine->GetSceneManager();
		InputManager *inputManager = engine->GetInputManager();
		PhysicsEngine* physics = engine->GetPhysicsEngine();
		GuiManager* guiManager = engine->GetGuiManager();
		ActionManager* actionManager = engine->GetActionManager();
		
		CameraLogic cameraLogic(*smgr, *actionManager);
		HUDLogic hudLogic(guiManager, actionManager, cameraLogic);
		hudLogic.setActiveHUD(HUDType::MainMenuHUD);
		
		VehicleHovering hovering(engine->GetPhysicsEngine());

		Scenario scenario = ScenarioCreator::CreateDemoScenario();
		ScenarioLoader::LoadScenario(engine.get(), scenario);
		int skip = 0;

		scenario.goal->bindHUD(&hudLogic);
		
		Planet* lastClosestPlanet = nullptr;
		Vector3<> lastForward(0.0f, 0.0f, 1.0f);
		while (engine->Run([&skip, inputManager, physics, &hovering, &cameraLogic, &lastClosestPlanet, &lastForward, scenario, &hudLogic, &actionManager]() {

			Vector3<> up(0.0f, 1.0f, 0.0f);

			Planet* closestPlanet = nullptr;

			if (skip > 1)
			{
				Vehicle* vehicle = scenario.vehicle;
				hudLogic.Update(vehicle->root->physics->velocity);
				Vector3<> vehiclePos = vehicle->root->localTransform.GetPosition();

				if (inputManager->IsKeyPressed(Key::P) && find(actionManager->GetState().begin(), actionManager->GetState().end(), "menuContext") == actionManager->GetState().end())
				{
					hudLogic.setActiveHUD(HUDType::PauseMenuHUD);
				}

				float distanceMin = numeric_limits<float>::max();
				for (auto& planet : scenario.gravityGenerators)
				{
					auto planetPos = planet->root->localTransform.GetPosition();
					auto planetRadius = planet->root->localTransform.GetScale().x / 2;
					auto distance = Vector3<>::Distance(vehiclePos, planetPos) - planetRadius;

					if (distance < distanceMin)
					{
						distanceMin = distance;
						closestPlanet = planet;
					}
				}

				vehicle->gravityApplied = Vector3<>(0.0f, 1.0f, 0.0f);
				if (closestPlanet->isUpVectorDynamic)
				{
					vehicle->gravityApplied = vehiclePos - closestPlanet->gravityCenter;
				}

				vehicle->gravityApplied.Normalize();
				up = vehicle->gravityApplied;
				vehicle->gravityApplied *= closestPlanet->gravityValue;
				vehicle->root->physics->addForce(vehicle->gravityApplied);

				hovering.Update(vehicle, closestPlanet->gravityValue, up);
			}

			skip++;

			if (lastClosestPlanet != closestPlanet)
			{
				Quaternion<> rot = scenario.vehicle->root->localTransform.GetRotation();
				Vector3<> vehicleForward = rot * Vector3<>{ 0.0f, 0.0f, 1.0f };
				Vector3<> refRight = Vector3<>::CrossProduct(up, vehicleForward);
				lastForward = Vector3<>::CrossProduct(refRight, up);
			}
			else
			{
				Vector3<> planeRight = Vector3<>::CrossProduct(up, lastForward);
				lastForward = Vector3<>::CrossProduct(planeRight, up);
			}

			lastForward.Normalize();

			if (inputManager->IsKeyPressed(Key::Alpha1))
			{
				cameraLogic.SetActiveCamera(CameraType::FirstPerson);
			}
			else if (inputManager->IsKeyPressed(Key::Alpha2))
			{
				cameraLogic.SetActiveCamera(CameraType::ThirdPerson);
			}
			else if (inputManager->IsKeyPressed(Key::Alpha3))
			{
				cameraLogic.SetActiveCamera(CameraType::FreeCam);
			}

			Vector2<int> mouseDelta = inputManager->GetMouseDelta();
			if (inputManager->IsMouseButtonPressed(Mouse::LeftButton))
			{
				auto rotations = cameraLogic.ThirdGetRotations();
				rotations.first += mouseDelta.x * 0.005f;
				rotations.second += mouseDelta.y * 0.005f;
				cameraLogic.ThirdSetRotations(rotations.first, rotations.second);
			}
			camDistance += -inputManager->GetMouseWheelDelta() * 4.0f;
			if (camDistance < 8.0f)
			{
				camDistance = 8.0f;
			}
			if (camDistance > 100.0f)
			{
				camDistance = 100.0f;
			}
			cameraLogic.ThirdSetDistance(camDistance);
			cameraLogic.Update(up, lastForward, scenario.vehicle->root->localTransform.GetPosition(), 0.2f);

			auto rot = Matrix4x4<>::FromRotation(scenario.vehicle->root->localTransform.GetRotation());
			Vector3<> vehicleForward = rot * Vector3<>{ 0.0f, 0.0f, 1.0f };
			Vector3<> vehicleRight = rot * Vector3<>{ 1.0f, 0.0f, 0.0f };
			Vector4<> vehicleUp = Vector4<>::CrossProduct(vehicleForward, vehicleRight);
			vehicleUp.Normalize();

			if (inputManager->IsKeyPressed(Key::W))
			{
				Vector3<> velocity = scenario.vehicle->root->physics->velocity;

				auto [_, projVelocity] = Vector3<>::Projection(velocity, vehicleForward);

				if (projVelocity.Length() < 100.0f)
				{
					scenario.vehicle->root->physics->addForce(vehicleForward * 50.0f);
					scenario.vehicle->root->physics->isDirty = true;
				}
			}

			if (inputManager->IsKeyPressed(Key::A))
			{
				auto rot = Quaternion<>::FromDirection(-Math::Pi / 90.0f, vehicleUp);
				scenario.vehicle->root->localTransform.SetRotation(rot * scenario.vehicle->root->localTransform.GetRotation());
				scenario.vehicle->root->physics->isDirty = true;
			}

			if (inputManager->IsKeyPressed(Key::S))
			{
				scenario.vehicle->root->physics->addForce(-vehicleForward * 50.0f);
				scenario.vehicle->root->physics->isDirty = true;
			}

			if (inputManager->IsKeyPressed(Key::D))
			{
				auto rot = Quaternion<>::FromDirection(Math::Pi / 90.0f, vehicleUp);
				scenario.vehicle->root->localTransform.SetRotation(rot * scenario.vehicle->root->localTransform.GetRotation());
				scenario.vehicle->root->physics->isDirty = true;
			}

			lastClosestPlanet = closestPlanet;

			// Animation des teleporteurs (interpolation de la courbe)
			for_each(scenario.gravityGenerators.begin(), scenario.gravityGenerators.end(), [](Planet* p) {
				for_each(p->teleportElements.begin(), p->teleportElements.end(), [](Teleport* t) {
					t->run();
				});
			});
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