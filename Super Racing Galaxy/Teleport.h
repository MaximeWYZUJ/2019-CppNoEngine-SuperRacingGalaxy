#pragma once
#include <vector>
#include <chrono>
#include "Prefab.h"

class Teleport : public Prefab
{
	std::vector<Cookie::Transform<>> controlPoints; // useful to interpolate the curve between two teleports
	std::chrono::system_clock::time_point lastUse{};
	float cooldown;
	double timeTravel;
	double realTimeTravel = 0.0;

	Cookie::Transform<> animateTeleport(double t);

public:
	Teleport* linkedTeleport;
	bool isActive = false;
	Prefab* objToTeleport = nullptr;

	Teleport(Cookie::Transform<> transform, std::string meshPath, std::wstring texturePath, double timeTravel = 2.0, float cooldown = 5.0f);
	void linkTo(Teleport* teleport, std::vector<Cookie::Transform<>> controlPoints_);
	void resetCooldown();
	bool mayUse() const;
	void run();
};
