#pragma once
#include <vector>
#include <chrono>
#include "Prefab.h"

class Teleport : public Prefab
{
	std::vector<Cookie::Transform<>> controlPoints; // useful to interpolate the curve between two teleports
	std::chrono::system_clock::time_point lastUse{};
	float cooldown;

public:
	Teleport* linkedTeleport;

	Teleport(Cookie::Transform<> transform, std::string meshPath, std::wstring texturePath, float cooldown = 5.0f, std::vector<Cookie::Transform<>> controlPoints = {});
	void linkTo(Teleport* teleport);
	void useNow();
	bool mayUse() const;
};
