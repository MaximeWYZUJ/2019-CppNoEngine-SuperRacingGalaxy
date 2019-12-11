#include "pch.h"
#include "DemoScenario.h"
#include "Planet.h"
#include "Vehicle.h"
#include "Vector3.h"

using namespace Cookie;

Scenario ScenarioCreator::CreateDemoScenario()
{
	Scenario scenario;

	scenario.gravityGenerators.push_back(new Planet(Transform<>({ 0.0f, 0.0f, 200.0f }, { 1000.0f, 1000.0f, 1000.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }),
										  -9.81f * 4.0f, true, "newPlanet.obj", L"textureNewPlanet.dds"));

	/*scenario.gravityGenerators.push_back(new Planet(Transform<>({ 3000.0f, 0.0f, 3000.0f }, { 1000.0f, 1000.0f, 1000.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }),
										  -15.0f, true, "newPlanet.obj", L"textureNewPlanet.dds"));

	scenario.gravityGenerators.push_back(new Planet(Transform<>({ 2000.0f, 1320.0f, 0.0f }, { 1000.0f, 1000.0f, 1000.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }),
									      -5.0f, true, "newPlanet.obj", L"textureNewPlanet.dds"));

	scenario.gravityGenerators.push_back(new Planet(Transform<>({ 1200.0f, 1200.0f, 0.0f }, { 100.0f, 100.0f, 100.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }),
										  -50.f, true, "newPlanet.obj", L"textureNewPlanet.dds"));

	scenario.gravityGenerators.push_back(new Planet(Transform<>({ 2000.0f, 0.0f, 7000.0f }, { 5000.0f, 5000.0f, 5000.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }),
										  -30.0f, true, "newPlanet.obj", L"textureNewPlanet.dds"));*/

	scenario.gravityGenerators.push_back(new Planet(Transform<>({ 0.0f, 1100.0f, 0.0f }, { 100.0f, 50.0f, 150.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }), 
										   -30.0f, false, "cargo.obj", L"textureCargo.dds"));

	scenario.vehicle = new Vehicle(Transform<>({ 0.0f, 1110.0f, 0.0f }, { 20.f, 8.f, 20.f }, { 0.0f, 0.0f, 0.0f, 1.0f }), "ultimateShip.obj", L"textureUltimateShip.dds");
	

	return scenario;
}
