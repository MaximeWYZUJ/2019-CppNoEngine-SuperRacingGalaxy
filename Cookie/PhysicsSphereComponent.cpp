#include "pch.h"
#include "PhysicsSphereComponent.h"

namespace Cookie {
	PhysicsSphereComponent::PhysicsSphereComponent(Vector3<PhysicsComponent_t> pos, Quaternion<PhysicsComponent_t> rot, PhysicMaterial mat, BodyType type_, float radius_, bool trigger_)
	{
		transform.SetPosition(pos);
		transform.SetRotation(rot);

		material = mat;

		type = type_;
		trigger = trigger_;

		radius = radius_;

		onCollisionCallback = new PhysicsCollisionCallback();
		onTriggerCallback = new PhysicsCollisionCallback();
	}

	PhysicsSphereComponent::~PhysicsSphereComponent()
	{
		delete onCollisionCallback;
		delete onTriggerCallback;
	}

	PhysicsComponent::ShapeType Cookie::PhysicsSphereComponent::getShapeType()
	{
		return ShapeType::SPHERE;
	}
}