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
#include "GuiManager.h"
#include "CompilerFlags.h"
#include "BillboardToggler.h"
#include "Cargo.h"

#undef max

using namespace std;
using namespace Cookie;
using namespace Srg;

float camDistance = 8.0f;

int main(int argc, char* argv[])
{
	if constexpr (!isDebug)
	{
		::ShowWindow(::GetConsoleWindow(), SW_HIDE);
	}
	
	try
	{
		unique_ptr<Engine> engine = EntryPoint::CreateStandaloneEngine();

		SceneManager* smgr = engine->GetSceneManager();
		InputManager *inputManager = engine->GetInputManager();
		PhysicsEngine* physics = engine->GetPhysicsEngine();
		GuiManager* guiManager = engine->GetGuiManager();
		ActionManager* actionManager = engine->GetActionManager();
		PostEffectManager* postEffectManager = engine->GetPostEffectManager();
		
		CameraLogic cameraLogic(*smgr, *actionManager);
		
		VehicleHovering hovering(engine->GetPhysicsEngine());

		Scenario scenario = ScenarioCreator::CreateDemoScenario();
		ScenarioLoader::LoadScenario(engine.get(), scenario);

		BillboardToggler billboardToggler(scenario.gravityGenerators);

		HUDLogic hudLogic(guiManager, actionManager, cameraLogic, scenario, engine.get());
		hudLogic.setActiveHUD(HUDType::MainMenuHUD);

		scenario.goal->bindHUD(&hudLogic);
		
		Planet* lastClosestPlanet = nullptr;
		Vector3<> lastForward(0.0f, 0.0f, 1.0f);
		while (engine->Run([inputManager, physics, &hovering, &cameraLogic, &billboardToggler, &lastClosestPlanet, &lastForward, &scenario, &hudLogic, &postEffectManager]() {

			Planet* closestPlanet = nullptr;

			Vehicle* vehicle = scenario.vehicle;
			if (vehicle->root->physics->velocity.Length() * 1.8 > 141.62f)
				postEffectManager->activatePostEffect(PostEffectManager::PostEffectType::RadialBlur);
			else postEffectManager->deactivatePostEffect(PostEffectManager::PostEffectType::RadialBlur);

			hudLogic.Update();
			Vector3<> vehiclePos = vehicle->root->localTransform.GetPosition();

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

			auto cargoDistance = Vector3<>::Distance(vehiclePos, scenario.cargo->root->localTransform.GetPosition());
			bool isCargoClosest = cargoDistance < distanceMin;

			vehicle->gravityApplied = Vector3<>(0.0f, 1.0f, 0.0f);
			if (!isCargoClosest)
			{
				vehicle->gravityApplied = vehiclePos - closestPlanet->gravityCenter;
			}

			auto gravityValue = isCargoClosest ? scenario.cargo->gravityValue : closestPlanet->gravityValue;

			vehicle->gravityApplied.Normalize();
			Vector3<> up = vehicle->gravityApplied;
			vehicle->gravityApplied *= gravityValue;
			vehicle->root->physics->addForce(vehicle->gravityApplied);
			
			bool isPlayerMoving = inputManager->IsKeyPressed(Key::W) || inputManager->IsKeyPressed(Key::A) || inputManager->IsKeyPressed(Key::S) || inputManager->IsKeyPressed(Key::D);

			hovering.Update(vehicle, gravityValue, up, isPlayerMoving);

			float changingPlanet = 1.0f;
			if (lastClosestPlanet != closestPlanet)
			{
				Quaternion<> rot = scenario.vehicle->root->localTransform.GetRotation();
				Vector3<> vehicleForward = rot * Vector3<>{ 0.0f, 0.0f, 1.0f };
				Vector3<> refRight = Vector3<>::CrossProduct(up, vehicleForward);
				lastForward = Vector3<>::CrossProduct(refRight, up);
				changingPlanet = 0.1f;
			}
			else
			{
				Vector3<> planeRight = Vector3<>::CrossProduct(up, lastForward);
				lastForward = Vector3<>::CrossProduct(planeRight, up);
			}

			changingPlanet += 0.01f;
			if (changingPlanet >= 1.0f)
			{
				changingPlanet = 1.0f;
			}

			lastForward.Normalize();

			Vector2<int> mouseDelta = inputManager->GetMouseDelta();
			if (inputManager->IsMouseButtonPressed(Mouse::LeftButton))
			{
				auto rotations = cameraLogic.ThirdGetRotations();
				rotations.first += mouseDelta.x * 0.005f;
				rotations.second += mouseDelta.y * 0.005f;
				cameraLogic.ThirdSetRotations(rotations.first, rotations.second);
			}
			camDistance += -inputManager->GetMouseWheelDelta() * 4.0f;
			if (camDistance < 4.0f)
			{
				camDistance = 4.0f;
			}
			if (camDistance > 100.0f)
			{
				camDistance = 100.0f;
			}
			cameraLogic.ThirdSetDistance(camDistance);
			cameraLogic.Update(up, lastForward, scenario.vehicle->root->localTransform, 0.15f * changingPlanet);


			lastClosestPlanet = closestPlanet;

			// Animation des teleporteurs (interpolation de la courbe)
			if (hudLogic.getActiveHUD() == HUDType::InGameHUD) {
				for_each(scenario.gravityGenerators.begin(), scenario.gravityGenerators.end(), [](Planet* p) {
					for_each(p->teleportElements.begin(), p->teleportElements.end(), [](Teleport* t) {
						t->run();
					});
				});
			}

			billboardToggler.Update(cameraLogic.GetActivateCameraPosition(), 800.0f);
		})){}

		return (int)1;
	}

	catch (const std::exception & E)
	{
		auto f = ofstream("error.txt");
		f << "error!";
		f.close();
		const int BufferSize = 128;
		wchar_t message[BufferSize];

		size_t numCharacterConverted;
		mbstowcs_s(&numCharacterConverted, message, E.what(), BufferSize - 1);
		auto a = string(E.what());
		wstring msg(begin(a), end(a));
		::MessageBox(nullptr, msg.c_str(), L"Erreur", MB_ICONWARNING);

		return (int)99;
	}

	catch (int codeErreur)
	{
		auto f = ofstream("error.txt");
		f << "error code!" << codeErreur;
		f.close();
		
		wchar_t szErrMsg[MAX_LOADSTRING];

		//::LoadString(hInstance, codeErreur, szErrMsg, MAX_LOADSTRING);
		auto a = to_string(codeErreur);
		wstring msg(begin(a), end(a));
		::MessageBox(nullptr, msg.c_str(), L"Erreur", MB_ICONWARNING);

		return (int)99; // POURQUOI 99???
	}
}