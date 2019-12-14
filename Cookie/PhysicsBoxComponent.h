#pragma once

#include "PhysicsComponent.h"
#include "ExportMacro.h"

namespace Cookie {
	struct COOKIE_API PhysicsBoxComponent : public PhysicsComponent
	{
		float dx = 1, dy = 1, dz = 1;

		PhysicsBoxComponent(Vector3<PhysicsComponent_t> pos, Quaternion<PhysicsComponent_t> rot, PhysicMaterial mat, BodyType type, float dx, float dy, float dz, bool trigger = false);
		~PhysicsBoxComponent();

		ShapeType getShapeType();
	};
}
