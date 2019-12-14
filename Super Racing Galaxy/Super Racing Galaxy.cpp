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

#undef max

using namespace std;
using namespace Cookie;
using namespace Srg;

float camDistance = 45.0f;

std::pair<float, Vector3<>> Projection(Vector3<> a, Vector3<> b)
{
	auto bLength = b.Length();
	auto dot = Vector3<>::DotProduct(b, a);
	return { dot, dot / Vector3<>::DotProduct(b, a) / (bLength * bLength) * b };
}

Vector3<> ComputeRepulsion(Vector3<> raycast, float hitDistance, float maxHitDistance, float planetGravity, Vector3<> curVelocity)
{
	Vector3<> force = Vector3<>::Normalize(-raycast);
	float heightInv = 1.0f - hitDistance / maxHitDistance;
	float forceFactor = heightInv * heightInv * heightInv * heightInv; // x^4
	float maxPulsion = planetGravity * -2.0f; // 2 times the gravity
	Vector3<> repulsion = force * forceFactor * maxPulsion;
	auto projection = Projection(curVelocity, repulsion);

	if (projection.first > 0.0f)
	{
		repulsion *= 0.5f;
	}

	return repulsion;
}

int main(int argc, char* argv[])
{
	try
	{
		unique_ptr<Engine> engine = EntryPoint::CreateStandaloneEngine();

		SceneManager* smgr = engine->GetSceneManager();
		InputManager *inputManager = engine->GetInputManager();
		PhysicsEngine* physics = engine->GetPhysicsEngine();

		GuiManager* guiManager = engine->GetGuiManager();
		
		
		//guiManager->newSprite("tree02S.dds", -607, 0);
		
		Gdiplus::Font* font = new Gdiplus::Font(new Gdiplus::FontFamily(L"Comic Sans MS", nullptr), 40.0f, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);
		Text* text1 = guiManager->newText(200, 50, font, L" 000 km/h", 0, 50);
		Text* text2 = guiManager->newText(200, 50, font, L" 00 : 00", (guiManager->ScreenWidth - 200)/2, 50);
		//guiManager->newSprite("tree02S.dds", 0, 200);
		
		CameraLogic cameraLogic(*smgr);
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

				// Raycasts
				Vector3<> vehicleScale = vehicle->root->localTransform.GetScale();
				Quaternion<> vehicleRot = vehicle->root->localTransform.GetRotation();
				float rayY = -vehicleScale.y / 2.0f - 0.1f;
				Vector3<> frontLeft = vehicleRot * Vector3<>(-vehicleScale.x / 2.0f, rayY, vehicleScale.z / 2.0f) + vehiclePos;
				Vector3<> frontRight = vehicleRot * Vector3<>(vehicleScale.x / 2.0f, rayY, vehicleScale.z / 2.0f) + vehiclePos;
				Vector3<> backLeft = vehicleRot * Vector3<>(-vehicleScale.x / 2.0f, rayY, -vehicleScale.z / 2.0f) + vehiclePos;
				Vector3<> backRight = vehicleRot * Vector3<>(vehicleScale.x / 2.0f, rayY, -vehicleScale.z / 2.0f) + vehiclePos;

				Vector3<> frontLeftRay = vehicleRot * Vector3<>(0.0f, -1.0f, 0.0f);
				Vector3<> frontRightRay = vehicleRot * Vector3<>(0.0f, -1.0f, 0.0f);
				Vector3<> backLeftRay = vehicleRot * Vector3<>(0.0f, -1.0f, 0.0f);
				Vector3<> backRightRay = vehicleRot * Vector3<>(0.0f, -1.0f, 0.0f);

				auto a = physics->PlanetRaycast(frontLeft, frontLeftRay, 2.0f);
				auto b = physics->PlanetRaycast(frontRight, frontRightRay, 2.0f);
				auto c = physics->PlanetRaycast(backLeft, backLeftRay, 2.0f);
				auto d = physics->PlanetRaycast(backRight, backRightRay, 2.0f);

				Vector3<> curVelocity = vehicle->root->physics->velocity;
				if (a.first)
				{
					vehicle->root->physics->addForce(ComputeRepulsion(frontLeftRay, a.second, 2.0f, closestPlanet->gravityValue, curVelocity));
					vehicle->root->physics->isDirty = true;
				}

				if (b.first)
				{
					vehicle->root->physics->addForce(ComputeRepulsion(frontRightRay, b.second, 2.0f, closestPlanet->gravityValue, curVelocity));
					vehicle->root->physics->isDirty = true;
				}

				if (c.first)
				{
					vehicle->root->physics->addForce(ComputeRepulsion(backLeftRay, c.second, 2.0f, closestPlanet->gravityValue, curVelocity));
					vehicle->root->physics->isDirty = true;
				}

				if (d.first)
				{
					vehicle->root->physics->addForce(ComputeRepulsion(backRightRay, d.second, 2.0f, closestPlanet->gravityValue, curVelocity));
					vehicle->root->physics->isDirty = true;
				}

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

			Vector2<int> mouseDelta = inputManager->GetMouseDelta();
			if (inputManager->IsMouseButtonPressed(MouseButton::LeftMouseButton))
			{
				auto rotations = cameraLogic.ThirdGetRotations();
				rotations.first += mouseDelta.x * 0.005f;
				rotations.second += mouseDelta.y * 0.005f;
				cameraLogic.ThirdSetRotations(rotations.first, rotations.second);
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

				auto [_, projVelocity] = Projection(velocity, vehicleForward);

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