#include "Vehicle.h"

Vehicle::Vehicle(Cookie::Transform<> transform, std::string meshPath, std::wstring texturePath)
{
	transform_ = transform;
	type_ = VEHICLE;
	meshPath_ = meshPath;
	texturePath_ = texturePath;
	mesh = nullptr;
	texture = nullptr;
}
