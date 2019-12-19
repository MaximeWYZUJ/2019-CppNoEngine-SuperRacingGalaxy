#pragma once
#include <vector>
#include <chrono>
#include <ostream>
#include "Prefab.h"

namespace Cookie {
	class PostEffectManager;
}

class Landing;

class Teleport : public Prefab
{
	std::vector<Cookie::Vector3<>> controlPoints; // useful to interpolate the curve between two teleports
	std::chrono::system_clock::time_point lastUse{};
	float cooldown;
	double timeTravel;
	double realTimeTravel = 0.0;
	Cookie::PostEffectManager* shaderManager;

	Cookie::Vector3<> animateTeleport(double t);

public:
	bool isActive = false;
	Prefab* objToTeleport = nullptr;

	Teleport(Cookie::Transform<> transform, std::string meshPath, std::wstring texturePath, std::string triggerMeshPath = "", double timeTravel = 20.0, float cooldown = 5.0f);
	void linkTo(Landing* teleport, std::vector<Cookie::Vector3<>> controlPoints_);
	void setShaderManager(Cookie::PostEffectManager* postEffectManager);
	void resetCooldown();
	bool mayUse() const;
	void run();

	void printControlPoints(std::ostream& out);
};
