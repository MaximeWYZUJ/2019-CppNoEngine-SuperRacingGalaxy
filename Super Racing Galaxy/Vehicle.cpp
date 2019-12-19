#include "Vehicle.h"

Vehicle::Vehicle(Cookie::Transform<> transform, std::string meshPath, std::wstring texturePath)
{
	initialTransform = transform;
	type_ = Type::VEHICLE;
	meshPath_ = meshPath;
	texturePath_ = texturePath;
	mesh = nullptr;
	texture = nullptr;
}

void Vehicle::useImpulse()
{
	lastImpulse = std::chrono::system_clock::now();
}

bool Vehicle::mayUseImpulse()
{
	auto now = std::chrono::system_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - lastImpulse).count();
	return duration > cooldownImpulse;
}
