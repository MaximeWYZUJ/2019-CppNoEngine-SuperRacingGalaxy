#pragma once

#include "PhysicComponent.h"
#include "ExportMacro.h"

namespace Cookie {
	class COOKIE_API PhysicBoxComponent : public PhysicComponent
	{
	private:
		float dx = 1, dy = 1, dz = 1;

	public:
		PhysicBoxComponent(Vector3<PhysicComponent_t> pos, Quaternion<PhysicComponent_t> rot, PhysicMaterial mat, bodyType type, float dx, float dy, float dz);
	};
}
