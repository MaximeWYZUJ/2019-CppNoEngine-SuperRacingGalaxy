#pragma once

#include <vector>

class Prefab;

class Scenario
{
public:
	std::vector<Prefab*> objects;
	Prefab* vehicle;
};
