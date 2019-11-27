#pragma once

#include <vector>

class Prefab;

class Scenario
{
public:
	std::vector<Prefab*> planets;
	std::vector<Prefab*> sceneries;
	Prefab* vehicle;
};
