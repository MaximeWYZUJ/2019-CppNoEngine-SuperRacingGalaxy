#include "pch.h"
#include "DemoScenario.h"
#include "Planet.h"
#include "Vehicle.h"
#include "Teleport.h"
#include "Scenery.h"
#include "Vector3.h"

using namespace Cookie;

Scenario ScenarioCreator::CreateDemoScenario()
{
	Scenario scenario;

	Planet *planete1 = new Planet(Transform<>({ 0.0f, 0.0f, 50.0f }, { 100.0f, 100.0f, 100.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }),
		-9.81f * 10.0f, true, "graphics\\meshs\\planete1.obj", L"graphics\\textureDDS\\planete1\\planete1Alpha.dds");

	{
		planete1->addElement(new Scenery(Transform<>({ 18.5f, 56.8f, 30.3f }, { 36.7f, 42.4f, 44.5f }, { 0.276956f, 0.473021f, -0.099187f, 0.830487f }),
			"graphics\\meshs\\champi2.obj", L"graphics\\textureDDS\\champi2Texture.dds"));
	}


	scenario.gravityGenerators.push_back(planete1);

	/*scenario.gravityGenerators.push_back(new Planet(Transform<>({ 3000.0f, 0.0f, 3000.0f }, { 1000.0f, 1000.0f, 1000.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }),
										  -15.0f, true, "newPlanet.obj", L"textureNewPlanet.dds"));

	scenario.gravityGenerators.push_back(new Planet(Transform<>({ 2000.0f, 1320.0f, 0.0f }, { 1000.0f, 1000.0f, 1000.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }),
									      -5.0f, true, "newPlanet.obj", L"textureNewPlanet.dds"));

	scenario.gravityGenerators.push_back(new Planet(Transform<>({ 1200.0f, 1200.0f, 0.0f }, { 100.0f, 100.0f, 100.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }),
										  -50.f, true, "newPlanet.obj", L"textureNewPlanet.dds"));

	scenario.gravityGenerators.push_back(new Planet(Transform<>({ 2000.0f, 0.0f, 7000.0f }, { 5000.0f, 5000.0f, 5000.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }),
										  -30.0f, true, "newPlanet.obj", L"textureNewPlanet.dds"));*/

	scenario.gravityGenerators.push_back(new Planet(Transform<>({ 0.0f, 250.0f, 0.0f }, { 10.0f, 5.0f, 15.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }), 
										   -30.0f, false, "cargo.obj", L"textureCargo.dds"));

	scenario.vehicle = new Vehicle(Transform<>({ 0.0f, 252.0f, 0.0f }, { 4.f, 1.6f, 4.f }, { 0.0f, 0.0f, 0.0f, 1.0f }), "ultimateShip.obj", L"textureUltimateShip.dds");
	
	scenario.teleports.push_back(new Teleport(Transform<>({ 25.0f, 550.0f, 0.0f }, { 10.0f, 10.0f, 10.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }), "ultimateShip.obj", L"textureUltimateShip.dds"));
	scenario.teleports.push_back(new Teleport(Transform<>({ -25.0f, 550.0f, 0.0f }, { 10.0f, 10.0f, 10.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }), "ultimateShip.obj", L"textureUltimateShip.dds"));
	
	scenario.teleports[0]->linkedTeleport = scenario.teleports[1];
	scenario.teleports[1]->linkedTeleport = scenario.teleports[0];
	
	return scenario;
}
