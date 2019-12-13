#pragma once

#include "Prefab.h"

#include <vector>

class Scenery;

class Planet : public Prefab {
public:
	Cookie::Vector3<> gravityCenter;
	float gravityValue;
	bool isUpVectorDynamic;
	std::vector<Scenery*> setElements;

	Planet(Cookie::Transform<> transform, float gravityValue, bool isUpVectorDynamic,
		   std::string meshPath, std::wstring texturePath);

	void addElement(Scenery* scenery);
	void anime();
};