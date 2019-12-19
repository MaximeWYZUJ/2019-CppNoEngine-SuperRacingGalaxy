#pragma once
#include "Prefab.h"
#include <chrono>

class Vehicle : public Prefab
{
	std::chrono::system_clock::time_point lastImpulse{};
	float cooldownImpulse = 2.0f;
public:
	Cookie::Vector3<> gravityApplied;
	Vehicle(Cookie::Transform<> transform, std::string meshPath, std::wstring texturePath);

	void useImpulse();
	bool mayUseImpulse();
};