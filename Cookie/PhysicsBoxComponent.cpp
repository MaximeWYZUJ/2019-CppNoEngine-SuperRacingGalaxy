#include "pch.h"
#include "PhysicsBoxComponent.h"
#include "PhysicsEngine.h"
#include "PxPhysicsAPI.h"

using namespace physx;

namespace Cookie {
	PhysicsBoxComponent::PhysicsBoxComponent(Vector3<PhysicsComponent_t> pos, Quaternion<PhysicsComponent_t> rot, PhysicMaterial mat, BodyType type_, float _dx, float _dy, float _dz)
					  : dx(_dx), dy(_dy), dz(_dz)
	{
		transform = Transform<PhysicsComponent_t>();
		transform.SetPosition(pos);
		transform.SetRotation(rot);

		material = mat;

		type = type_;

		onCollisionCallback = new PhysicsCollisionCallback();
		onTriggerCallback = new PhysicsCollisionCallback();
	}
	PhysicsBoxComponent::~PhysicsBoxComponent()
	{
		delete onCollisionCallback;
		delete onTriggerCallback;
	}
	PhysicsComponent::ShapeType PhysicsBoxComponent::getShapeType()
	{
		return ShapeType::BOX;
	}
}
