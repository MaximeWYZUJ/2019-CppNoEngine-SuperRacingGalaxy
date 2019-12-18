#pragma once

#include "Prefab.h"

#include <vector>

class Scenery;
class Teleport;
class Goal;

class Planet : public Prefab {
public:
	Cookie::Vector3<> gravityCenter;
	float gravityValue;
	bool isUpVectorDynamic;
	std::vector<Scenery*> setElements;
	std::vector<Teleport *> teleportElements;
	Goal* goal;

	Planet(Cookie::Transform<> transform, float gravityValue, bool isUpVectorDynamic,
		   std::string meshPath, std::wstring texturePath);

	void addElement(Scenery* scenery);
	void addTeleport(Teleport *scenery);
	void addGoal(Goal *goal);
	void anime();
};