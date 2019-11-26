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

	scenario.objects.push_back(new Planet(Transform<>({ 0.0f, 0.0f, 0.0f }, { 100.0f, 100.0f, 100.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }),
										  -9.81f, Vector3<>(0, 1, 0), 5.f, "newPlanet.obj", L"textureNewPlanet.dds"));

	scenario.objects.push_back(new Planet(Transform<>({ 3000.0f, 0.0f, 3000.0f }, { 1000.0f, 1000.0f, 1000.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }),
										  -9.81f, Vector3<>(0, 1, 0), 5.f, "newPlanet.obj", L"textureNewPlanet.dds"));

	scenario.objects.push_back(new Planet(Transform<>({ 2000.0f, 1320.0f, 0.0f }, { 1000.0f, 1000.0f, 1000.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }),
									      -9.81f, Vector3<>(0, 1, 0), 5.f, "newPlanet.obj", L"textureNewPlanet.dds"));

	scenario.objects.push_back(new Planet(Transform<>({ 1200.0f, 1200.0f, 0.0f }, { 100.0f, 100.0f, 100.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }),
										  -9.81f, Vector3<>(0, 1, 0), 5.f, "newPlanet.obj", L"textureNewPlanet.dds"));

	scenario.objects.push_back(new Planet(Transform<>({ 2000.0f, 0.0f, 7000.0f }, { 5000.0f, 5000.0f, 5000.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }),
										  -9.81f, Vector3<>(0, 1, 0), 5.f, "newPlanet.obj", L"textureNewPlanet.dds"));

	scenario.objects.push_back(new Planet(Transform<>({ 0.0f, 1200.0f, 100.0f }, { 100.0f, 100.0f, 100.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }),
		-9.81f, Vector3<>(0, 1, 0), 5.f, "cargo.obj", L"UneTexture.dds"));

	scenario.vehicle = new Vehicle(Transform<>({ 0.0f, 200.0f, 100.0f }, { 2.f, 1.f, 1.f }, { 0.0f, 0.0f, 0.0f, 1.0f }), "vehicle.obj", L"UneTexture.dds");
	

	return scenario;
}
