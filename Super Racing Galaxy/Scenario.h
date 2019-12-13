#pragma once

#include <vector>

class Prefab;
class Planet;
class Scenery;
class Vehicle;

class Scenario
{
public:
	std::vector<Planet*> gravityGenerators;
	std::vector<Scenery*> sceneries;
	Vehicle* vehicle;
};
