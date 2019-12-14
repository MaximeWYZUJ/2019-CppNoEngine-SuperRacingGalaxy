#pragma once

#include "PhysicsComponent.h"
#include "ExportMacro.h"
#include "Mesh.h"

namespace Cookie {
	struct COOKIE_API PhysicsMeshComponent : public PhysicsComponent
	{
		Mesh& mesh;

		PhysicsMeshComponent(Vector3<PhysicsComponent_t> pos, Quaternion<PhysicsComponent_t> rot, PhysicMaterial mat, BodyType type, Mesh& mesh, Vector3<> size, bool trigger = false);
		~PhysicsMeshComponent();

		ShapeType getShapeType();
	};
}
