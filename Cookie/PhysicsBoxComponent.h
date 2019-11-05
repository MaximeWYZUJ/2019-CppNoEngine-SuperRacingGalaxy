#pragma once

#include "PhysicsComponent.h"
#include "ExportMacro.h"

namespace Cookie {
	class COOKIE_API PhysicsBoxComponent : public PhysicsComponent
	{
	private:
		float dx = 1, dy = 1, dz = 1;

	public:
		PhysicsBoxComponent(Vector3<PhysicsComponent_t> pos, Quaternion<PhysicsComponent_t> rot, PhysicMaterial mat, bodyType type, float dx, float dy, float dz);
	};
}
