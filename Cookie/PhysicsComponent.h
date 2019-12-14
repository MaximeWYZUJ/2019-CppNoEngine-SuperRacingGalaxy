#pragma once
#include "ExportMacro.h"
#include "Transform.h"
#include "PhysicMaterial.h"
#include "PhysicsCollisionCallback.h"
#include "Component.h"
#include "PhysicsCollisionFiltering.h"
#include <set>
#include <vector>

namespace physx
{
	class PxRigidActor;
}

namespace Cookie
{
	class NoMass_StaticObject_Exception {};
	class NoVelocity_StaticObject_Exception {};
	class NoShapeException {};

	class COOKIE_API PhysicsComponent : public Component
	{
	public:
		enum BodyType { STATIC, DYNAMIC };
		enum ShapeType { BOX, SPHERE, MESH };
		using PhysicsComponent_t = float;

		Transform<PhysicsComponent_t> transform;
		Vector3<PhysicsComponent_t> velocity;
		Vector3<PhysicsComponent_t> massCenter{};
		PhysicsComponent_t mass = 1;
		BodyType type;
		bool trigger = false;
		PhysicMaterial material;

		std::set<FilterGroup> selfGroup;
		std::set<FilterGroup> mask;

		physx::PxRigidActor* actor = nullptr;

		PhysicsCollisionCallback* onCollisionCallback = nullptr;
		PhysicsCollisionCallback* onTriggerCallback = nullptr;

		bool isDirty = false;

		void* userData = nullptr; // meh... (pointeur sur le prefab en pratique)

	protected:
		std::vector<Vector3<PhysicsComponent_t>> addedForces;

	public:
		virtual ShapeType getShapeType() = 0;

		void addFilterGroup(FilterGroup f);
		void removeFilterGroup(FilterGroup f);
		void addFilterMask(FilterGroup f);
		void removeFilterMask(FilterGroup f);
		void addForce(Vector3<PhysicsComponent_t> force);
		void SetAngularVelocity(Vector3<PhysicsComponent_t> velocity);

		template <class T>
		void changeCollisionCallback();
		template <class T>
		void changeTriggerCallback();
	};
	template<class T>
	inline void PhysicsComponent::changeCollisionCallback()
	{
		delete onCollisionCallback;
		onCollisionCallback = new T{};
	}
	template<class T>
	inline void PhysicsComponent::changeTriggerCallback()
	{
		delete onTriggerCallback;
		onTriggerCallback = new T{};
	}
}