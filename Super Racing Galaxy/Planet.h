#pragma once

#include "Prefab.h"

#include <vector>

class Scenery;
class Teleport;

class Planet : public Prefab {
public:
	Cookie::Vector3<> gravityCenter;
	float gravityValue;
	bool isUpVectorDynamic;
	std::vector<Scenery*> setElements;
	std::vector<Teleport *> teleportElements;

	Planet(Cookie::Transform<> transform, float gravityValue, bool isUpVectorDynamic,
		   std::string meshPath, std::wstring texturePath);

	void addElement(Scenery* scenery);
	void addTeleport(Teleport *scenery);
	void anime();
};