#pragma once

#include <vector>

#include "Transform.h"
#include "PhysicMaterial.h"
#include "PhysicCollisionCallback.h"
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

	class COOKIE_API PhysicComponent
	{
	public:
		enum bodyType {
			STATIC, DYNAMIC
		};

		using PhysicComponent_t = float;

	protected:
		Vector3<PhysicComponent_t> massCenter{};
		bodyType type;
		bool trigger = false;
		PhysicMaterial material;

		std::vector<FilterGroup> selfGroup;
		std::vector<FilterGroup> mask;

		physx::PxRigidActor* actor = nullptr;

	public:
		~PhysicComponent() = default;

		void addForce(Vector3<PhysicComponent_t> force);
		PhysicCollisionCallback onCollisionCallBack;
		PhysicCollisionCallback onTriggerCallBack;

		void addFilterGroup(FilterGroup f);
		void removeFilterGroup(FilterGroup f);
		void addFilterMask(FilterGroup f);
		void removeFilterMask(FilterGroup f);

	private:
		void updateFilters();

	public:
		virtual Transform<PhysicComponent_t> getTransform() const noexcept;
		virtual PhysicComponent_t getMass() const;
		virtual Vector3<PhysicComponent_t> getVelocity() const;
		virtual PhysicMaterial getMaterial() const noexcept;
		virtual bool isTrigger() const noexcept;

		virtual void setMass(PhysicComponent_t mass);
		virtual void setMaterial(PhysicMaterial material_);
		virtual void setTrigger(bool isTrigger_);
	};
}