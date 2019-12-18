#pragma once

#include <vector>
#include <utility>
#include "Vector3.h"

class Prefab;
class Planet;
class Scenery;
class Vehicle;
class Skybox;
class Teleport;
class Landing;
class Goal;

struct TeleportLinksParams {
	Teleport* teleport;
	Planet* teleportPlanet;
	Landing* landing;
	Planet* landingPlanet;
	std::pair<bool, Cookie::Vector3<>> firstDefinedControlPoint;
	std::pair<bool, Cookie::Vector3<>> lastDefinedControlPoint;
};

class Scenario
{
public:
	std::vector<Planet*> gravityGenerators;
	std::vector<Scenery*> sceneries;
	std::vector<TeleportLinksParams> tpLinks;
	Vehicle* vehicle;
	Goal* goal;
	Skybox *skybox;
};
