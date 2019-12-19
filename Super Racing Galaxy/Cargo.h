#pragma once

#include "Prefab.h"

#include <vector>

class Scenery;
class Teleport;
class Goal;

class Cargo : public Prefab {
public:
	Cookie::Vector3<> gravityCenter;
	float gravityValue;
	bool isUpVectorDynamic;

	Cargo(Cookie::Transform<> transform, float gravityValue, bool isUpVectorDynamic, std::string meshPath, std::wstring texture1Path);

	void anime();
};