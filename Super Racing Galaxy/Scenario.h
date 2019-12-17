#pragma once

#include <vector>

class Prefab;
class Planet;
class Scenery;
class Vehicle;
class Skybox;
class Teleport;

class Scenario
{
public:
	std::vector<Planet*> gravityGenerators;
	std::vector<Scenery*> sceneries;
	Vehicle* vehicle;
	Skybox *skybox;
};
