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
