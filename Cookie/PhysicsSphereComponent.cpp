#include "pch.h"
#include "PhysicsSphereComponent.h"

namespace Cookie {
	PhysicsSphereComponent::PhysicsSphereComponent(Vector3<PhysicsComponent_t> pos, Quaternion<PhysicsComponent_t> rot, PhysicMaterial mat, BodyType type_, float radius_)
	{
		transform.SetPosition(pos);
		transform.SetRotation(rot);

		material = mat;

		type = type_;

		radius = radius_;
	}

	PhysicsComponent::ShapeType Cookie::PhysicsSphereComponent::getShapeType()
	{
		return ShapeType::SPHERE;
	}
}