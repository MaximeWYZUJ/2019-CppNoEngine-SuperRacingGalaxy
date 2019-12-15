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

	Planet *planete1 = new Planet(Transform<>({ 0.0f, 0.0f, 50.0f }, { 100.0f, 100.0f, 100.0f }, { 0.0f, 0.0f, 0.0f, 1.f }),
		-9.81f, true, "graphics\\meshs\\planete1test.obj", L"graphics\\textureDDS\\planete1\\planete1Alpha.dds");

	{
		planete1->addElement(new Scenery(Transform<>({ 0.200152f, 0.487519f, 0.311663f }, { 0.362121f, 0.419094f, 0.439271f }, { 0.28219f, 0.472567f, -0.106708f, 0.828047f }),
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

	scenario.gravityGenerators.push_back(new Planet(Transform<>({ 0.0f, 150.0f, 0.0f }, { 20.0f, 10.0f, 30.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }), 
										   -9.81f, false, "graphics\\meshs\\cargo.obj", L"graphics\\textureDDS\\textureCargo.dds"));

	scenario.vehicle = new Vehicle(Transform<>({ 0.0f, 152.0f, 0.0f }, { 4.f, 1.6f, 4.f }, { 0.0f, 0.0f, 0.0f, 1.0f }), "graphics\\meshs\\ultimateShip.obj", L"graphics\\textureDDS\\textureUltimateShip.dds");
	
	{
		Teleport* teleporteur1 = new Teleport(Transform<>({ 0.262297f, -0.298794f, 0.123498f }, { 0.246564f, 0.098625f, 0.246564f }, { -0.04615f, 0.108863f, 0.964831f, -0.234776f }), "graphics\\meshs\\teleporteur.obj", L"graphics\\textureDDS\\teleporteurTexture.dds");
		planete1->addTeleport(teleporteur1);

		Teleport *teleporteur2 = new Teleport(Transform<>({ 0.002394f, 0.308178f, -0.394474f }, { 0.246564f, 0.098626f, 0.246564f }, { 0.042097f, -0.853319f, 0.516571f, 0.056826f }), "graphics\\meshs\\teleporteur.obj", L"graphics\\textureDDS\\teleporteurTexture.dds");
		planete1->addTeleport(teleporteur2);

		teleporteur1->linkedTeleport = teleporteur2;
		teleporteur2->linkedTeleport = teleporteur1;
	}

	return scenario;
}
