#pragma once

#include <vector>

#include "Vector3.h"
#include "Quaternion.h"
#include "PhysicMaterial.h"
#include "PxActor.h"
#include "PxRigidBody.h"

using std::vector;

namespace Cookie
{
	class PhysicComponent
	{
	public:
		enum bodyType {
			STATIC, DYNAMIC
		};

	protected:
		Vector3<float> position;
		Quaternion rotation;
		float mass = 1;
		Vector3<float> velocity{};
		Vector3<float> massCenter{};
		PhysicMaterial material{};
		bool isTrigger = false;
		bodyType type;

		vector<FilterGroup> selfGroup;
		vector<FilterGroup> mask;

		physx::PxActor* actor;
		physx::PxRigidBody* body;

	public:
		PhysicComponent(Vector3<float> pos, Quaternion rot, bodyType type);
		~PhysicComponent() = default;

		virtual void addForce() = 0;
		virtual void onCollisionCallBack() = 0;
		virtual void onTriggerCallBack() = 0;

		virtual void addFilterGroup(FilterGroup f);
		virtual void removeFilterGroup(FilterGroup f);
		virtual void addFilterMask(FilterGroup f);
		virtual void removeFilterMask(FilterGroup f);
	};
}