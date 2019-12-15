#include "pch.h"

#include "EntryPoint.h"
#include "SceneManager.h"
#include <DeviceD3D11.h>
#include "Vector4.h"
#include "ScenarioLoader.h"
#include "DemoScenario.h"
#include "Vehicle.h"
#include "Planet.h"
#include "CameraLogic.h"
#include "VehicleHovering.h"
#include "Vector3.h"

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

		guiManager->newSprite("tree02S.dds", 0, 200);
		Gdiplus::Font* font = new Gdiplus::Font(new Gdiplus::FontFamily(L"Comic Sans MS", nullptr), 40.0f, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);
		Text* text1 = guiManager->newText(200, 50, font, L" 000 km/h", 0, 50);
		Text* text2 = guiManager->newText(200, 50, font, L" 00 : 00", (guiManager->ScreenWidth - 200)/2, 50);

		//Text* bouton = guiManager->newButton(200, 200, font, L"Test infini pour voir si c'est bien centré", 400, 300);
		
		CameraLogic cameraLogic(*smgr, *actionManager);
		cameraLogic.SetActiveCamera(CameraType::ThirdPerson);
		
		VehicleHovering hovering(engine->GetPhysicsEngine());
		
		Scenario scenario = ScenarioCreator::CreateDemoScenario();
		ScenarioLoader::LoadScenario(engine.get(), scenario);
		int speed = 0;
		int skip = 0;
		int min = 0;
		int sec = 0;
		wstring fill;
		wstring fill2;
		Planet* lastClosestPlanet = nullptr;
		Vector3<> lastForward(0.0f, 0.0f, 1.0f);
		while (engine->Run([&skip, inputManager, physics, &hovering, &cameraLogic, &lastClosestPlanet, &lastForward, scenario, &guiManager, &text1, &speed, &fill, &fill2, &sec, &min, &text2]() {

			Vector3<> up(0.0f, 1.0f, 0.0f);

			Planet* closestPlanet = nullptr;

			if (skip > 1)
			{
				Vehicle *vehicle = scenario.vehicle;
				Vector3<> velocity = vehicle->root->physics->velocity;
				Vector3<> vehiclePos = vehicle->root->localTransform.GetPosition();

				speed = round(velocity.Length() * 3.6);
				speed < 100 ? (speed < 10 ? fill = L"00" : fill = L"0") : fill = L"";
				
				guiManager->Write(fill + to_wstring(speed) + L" km/h", text1);

				if (skip % 60 == 0)
					++sec;
				if (sec == 60)
				{
					++min;
					sec = 0;
				}
				min < 10 ? fill = L"0" : fill = L"";
				sec < 10 ? fill2 = L"0" : fill2 = L"";
				guiManager->Write(fill + to_wstring(min) + L" : " + fill2 + to_wstring(sec), text2);
				
				float distanceMin = numeric_limits<float>::max();
				for (auto &planet : scenario.gravityGenerators)
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
			Vector3<> vehicleRight= rot * Vector3<>{ 1.0f, 0.0f, 0.0f };
			Vector4<> vehicleUp = Vector4<>::CrossProduct(vehicleForward, vehicleRight);
			vehicleUp.Normalize();
			
			if (inputManager->IsKeyPressed(Key::W))
			{
				Vector3<> velocity = scenario.vehicle->root->physics->velocity;

				auto [_, projVelocity] = Vector3<>::Projection(velocity, vehicleForward);

				if (projVelocity.Length() < 100.0f)
				{
					scenario.vehicle->root->physics->addForce(vehicleForward * 100.0f);
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
				scenario.vehicle->root->physics->addForce(-vehicleForward * 100.0f);
				scenario.vehicle->root->physics->isDirty = true;
			}

			if (inputManager->IsKeyPressed(Key::D))
			{
				auto rot = Quaternion<>::FromDirection(Math::Pi / 90.0f, vehicleUp);
				scenario.vehicle->root->localTransform.SetRotation(rot *scenario.vehicle->root->localTransform.GetRotation());
				scenario.vehicle->root->physics->isDirty = true;
			}

			lastClosestPlanet = closestPlanet;
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