#pragma once

#include "PhysicsComponent.h"
#include "ExportMacro.h"

namespace Cookie {
	struct COOKIE_API PhysicsSphereComponent : public PhysicsComponent
	{
		float radius = 1;

		PhysicsSphereComponent(Vector3<PhysicsComponent_t> pos, Quaternion<PhysicsComponent_t> rot, PhysicMaterial mat, BodyType type, float radius_);
		~PhysicsSphereComponent();

		ShapeType getShapeType();
	};
}