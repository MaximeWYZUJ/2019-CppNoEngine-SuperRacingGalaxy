#pragma once

#include "PhysicComponent.h"

namespace Cookie {
	class PhysicBoxComponent : public PhysicComponent
	{
	private:
		float dx = 1, dy = 1, dz = 1;

	public:
		PhysicBoxComponent(Vector3<float> pos, Quaternion<> rot, PhysicMaterial mat, bodyType type, float dx, float dy, float dz);
		virtual void addForce() override;
		virtual void onCollisionCallBack() override;
		virtual void onTriggerCallBack() override;
	};
}
