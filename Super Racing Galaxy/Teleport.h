#pragma once
#include <vector>
#include <chrono>
#include "Prefab.h"

class Landing;

class Teleport : public Prefab
{
	std::vector<Cookie::Vector3<>> controlPoints; // useful to interpolate the curve between two teleports
	std::chrono::system_clock::time_point lastUse{};
	float cooldown;
	double timeTravel;
	double realTimeTravel = 0.0;

	Cookie::Transform<> animateTeleport(double t);

public:
	Landing* linkedTeleport;
	bool isActive = false;
	Prefab* objToTeleport = nullptr;

	Teleport(Cookie::Transform<> transform, std::string meshPath, std::wstring texturePath, std::string triggerMeshPath = "", double timeTravel = 20.0, float cooldown = 5.0f);
	void linkTo(Landing* teleport, std::vector<Cookie::Vector3<>> controlPoints_);
	void resetCooldown();
	bool mayUse() const;
	void run();
};
