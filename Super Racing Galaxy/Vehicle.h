#pragma once

#include "Prefab.h"

class Vehicle : public Prefab {
public:

	Cookie::Vector3<> gravityApplied;
	Vehicle(Cookie::Transform<> transform, std::string meshPath, std::wstring texturePath);
};