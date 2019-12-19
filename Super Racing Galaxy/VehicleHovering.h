#pragma once
#include "Vector3.h"

namespace Cookie {
	class PhysicsEngine;
}

class Vehicle;

namespace Srg
{
	class VehicleHovering
	{
	public:
		VehicleHovering(Cookie::PhysicsEngine* physics);

		void Update(Vehicle* vehicle, float gravityValue, Cookie::Vector3<> planetUp, bool isPlayerMoving);

	private:
		Cookie::PhysicsEngine* physics;
	};
}
