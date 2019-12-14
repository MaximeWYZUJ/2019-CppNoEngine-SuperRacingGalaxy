#include "pch.h"
#include "PhysicsMeshComponent.h"

namespace Cookie {
	PhysicsMeshComponent::PhysicsMeshComponent(Vector3<PhysicsComponent_t> pos, Quaternion<PhysicsComponent_t> rot, PhysicMaterial mat, BodyType type_, Mesh& mesh_, Vector3<> size, bool trigger_)
		: mesh(mesh_)
	{
		transform = Transform<PhysicsComponent_t>();
		transform.SetPosition(pos);
		transform.SetRotation(rot);
		transform.SetScale(size);

		material = mat;

		type = type_;
		trigger = trigger_;

		onCollisionCallback = new PhysicsCollisionCallback();
		onTriggerCallback = new PhysicsCollisionCallback();
	}

	PhysicsMeshComponent::~PhysicsMeshComponent()
	{
		delete onCollisionCallback;
		delete onTriggerCallback;
	}

	PhysicsMeshComponent::ShapeType PhysicsMeshComponent::getShapeType()
	{
		return ShapeType::MESH;
	}
}