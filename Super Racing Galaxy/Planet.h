#pragma once

#include "Prefab.h"

#include <vector>

class Scenery;

class Planet : public Prefab {
public:
	Cookie::Vector3<> gravityCenter;
	float gravityValue;
	Cookie::Vector3<> rotDir;
	float rotSpeed;
	std::vector<Scenery*> setElements;

	Planet(Cookie::Transform<> transform, float gravityValue,
		   Cookie::Vector3<> rotDir, float rotSpeed,
		   std::string meshPath, std::wstring texturePath);

	void addElement(Scenery* scenery);
	void anime();
};