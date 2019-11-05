#pragma once

#include <set>
#include <vector>

#include "Transform.h"
#include "PhysicMaterial.h"
#include "PhysicsCollisionCallback.h"
#include "ExportMacro.h"

namespace physx
{
	class PxRigidActor;
}

namespace Cookie
{
	class NoMass_StaticObject_Exception {};
	class NoVelocity_StaticObject_Exception {};
	class NoShapeException {};

	struct COOKIE_API PhysicsComponent
	{
		enum BodyType { STATIC, DYNAMIC };
		using PhysicsComponent_t = float;

		Transform<PhysicsComponent_t> transform;
		Vector3<PhysicsComponent_t> velocity;
		Vector3<PhysicsComponent_t> massCenter{};
		PhysicsComponent_t mass;
		BodyType type;
		bool trigger = false;
		PhysicMaterial material;

		std::set<FilterGroup> selfGroup;
		std::set<FilterGroup> mask;

		physx::PxRigidActor* actor = nullptr;

		PhysicsCollisionCallback onCollisionCallback;
		PhysicsCollisionCallback onTriggerCallback;

		bool isDirty = false;

	protected:
		std::vector<Vector3<PhysicsComponent_t>> addedForces;

	public:
		void addFilterGroup(FilterGroup f);
		void removeFilterGroup(FilterGroup f);
		void addFilterMask(FilterGroup f);
		void removeFilterMask(FilterGroup f);
		void addForce(Vector3<PhysicsComponent_t> force);
	};

	/*class COOKIE_API PhysicsComponent
	{
	public:
		enum bodyType {
			STATIC, DYNAMIC
		};

		using PhysicsComponent_t = float;

	protected:
		Vector3<PhysicsComponent_t> massCenter{};
		bodyType type;
		bool trigger = false;
		PhysicMaterial material;

		std::vector<FilterGroup> selfGroup;
		std::vector<FilterGroup> mask;

		physx::PxRigidActor* actor = nullptr;

	public:
		~PhysicsComponent() = default;

		void addForce(Vector3<PhysicsComponent_t> force);
		PhysicsCollisionCallback onCollisionCallBack;
		PhysicsCollisionCallback onTriggerCallBack;

		void addFilterGroup(FilterGroup f);
		void removeFilterGroup(FilterGroup f);
		void addFilterMask(FilterGroup f);
		void removeFilterMask(FilterGroup f);

	private:
		void updateFilters();

	public:
		virtual Transform<PhysicsComponent_t> getTransform() const noexcept;
		virtual PhysicsComponent_t getMass() const;
		virtual Vector3<PhysicsComponent_t> getVelocity() const;
		virtual PhysicMaterial getMaterial() const noexcept;
		virtual bool isTrigger() const noexcept;

		virtual void setMass(PhysicsComponent_t mass);
		virtual void setMaterial(PhysicMaterial material_);
		virtual void setTrigger(bool isTrigger_);
	};*/
}