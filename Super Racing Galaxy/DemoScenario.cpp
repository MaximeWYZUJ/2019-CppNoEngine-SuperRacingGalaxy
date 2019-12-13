#include "pch.h"
#include "DemoScenario.h"
#include "Planet.h"
#include "Vehicle.h"
#include "Scenery.h"
#include "Vector3.h"

#include <corecrt_math_defines.h>

using namespace Cookie;

Scenario ScenarioCreator::CreateDemoScenario()
{
	Scenario scenario;

	scenario.planets.push_back(new Planet(Transform<>({ 0.0f, 0.0f, 200.0f }, { 1000.0f, 1000.0f, 1000.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }),
										  -50.f, Vector3<>(0, 1, 0), 5.f, "newPlanet.obj", L"textureNewPlanet.dds"));

	scenario.planets.push_back(new Planet(Transform<>({ 3000.0f, 0.0f, 3000.0f }, { 1000.0f, 1000.0f, 1000.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }),
										  -50.f, Vector3<>(0, 1, 0), 5.f, "newPlanet.obj", L"textureNewPlanet.dds"));

	scenario.planets.push_back(new Planet(Transform<>({ 2000.0f, 1320.0f, 0.0f }, { 1000.0f, 1000.0f, 1000.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }),
									      -50.f, Vector3<>(0, 1, 0), 5.f, "newPlanet.obj", L"textureNewPlanet.dds"));

	scenario.planets.push_back(new Planet(Transform<>({ 1200.0f, 1200.0f, 0.0f }, { 100.0f, 100.0f, 100.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }),
										  -100.f, Vector3<>(0, 1, 0), 5.f, "newPlanet.obj", L"textureNewPlanet.dds"));

	scenario.planets.push_back(new Planet(Transform<>({ 2000.0f, 0.0f, 7000.0f }, { 5000.0f, 5000.0f, 5000.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }),
										  -70.f, Vector3<>(0, 1, 0), 5.f, "newPlanet.obj", L"textureNewPlanet.dds"));

	scenario.sceneries.push_back(new Scenery(Transform<>({ 0.0f, 1500.0f, 100.0f }, { 100.0f, 50.0f, 150.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }), 
										   "cargo.obj", L"textureCargo.dds"));

	scenario.vehicle = new Vehicle(Transform<>({ 0.0f, 1505.0f, 100.0f }, { 8.f, 3.2f, 8.f }, { 0.0f, 1.0f, 0.0f, 0.0f }), "ultimateShip.obj", L"textureUltimateShip.dds");
	

	return scenario;
}
