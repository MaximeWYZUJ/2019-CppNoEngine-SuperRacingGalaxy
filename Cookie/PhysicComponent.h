#pragma once

#include <vector>

#include "Transform.h"
#include "PhysicMaterial.h"
#include "PxActor.h"
#include "PxRigidBody.h"
#include "PhysicEngine.h"
#include "PhysicCollisionCallback.h"

namespace Cookie
{
	class NoMass_StaticObject_Exception {};
	class NoVelocity_StaticObject_Exception {};
	class NoShapeException {};

	class PhysicComponent
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
		virtual inline Transform<PhysicComponent_t> getTransform() const noexcept {
			using namespace physx;

			PxTransform pxT = actor->getGlobalPose();
			Transform<PhysicComponent_t> t{};
			t.pos = Vector3<PhysicComponent_t>(-pxT.p.x, pxT.p.y, pxT.p.z);
			t.rotation = Quaternion<PhysicComponent_t>(pxT.q.x, -pxT.q.y, -pxT.q.z, pxT.q.w);
			t.scale = Vector3<PhysicComponent_t>(1.0f, 1.0f, 1.0f);

			return t;
		}
		virtual inline PhysicComponent_t getMass() const {
			using namespace physx;

			if (type == STATIC)
				throw NoMass_StaticObject_Exception{};

			return static_cast<PhysicComponent_t>(actor->is<PxRigidDynamic>()->getMass());
		}
		virtual inline Vector3<PhysicComponent_t> getVelocity() const {
			using namespace physx;

			if (type == STATIC)
				throw NoVelocity_StaticObject_Exception{};
			
			PxVec3 velocity = actor->is<PxRigidDynamic>()->getLinearVelocity();
			return Vector3<PhysicComponent_t>(-velocity.x, velocity.y, velocity.z);
		}
		virtual inline PhysicMaterial getMaterial() const noexcept {
			return material;
		}
		virtual inline bool isTrigger() const noexcept {
			return trigger;
		}

		virtual inline void setMass(PhysicComponent_t mass) {
			using namespace physx;

			if (type == STATIC)
				throw NoMass_StaticObject_Exception{};

			actor->is<PxRigidDynamic>()->setMass(static_cast<PxReal>(mass));
		}
		virtual inline void setMaterial(PhysicMaterial material_) {
			using namespace physx;

			int nbShapes = actor->getNbShapes();
			if (nbShapes == 0) {
				throw NoShapeException{};
			}

			PxShape** shapes{};
			actor->getShapes(shapes, sizeof(PxShape) * nbShapes);

			PxMaterial* mat = PhysicEngine::getInstance().gPhysics->createMaterial(material_.staticFriction, material_.dynamicFriction, material_.bounce);
			for (int i = 0; i < nbShapes; i++) {
				shapes[i]->setMaterials(&mat, 1);
			}

			material = material_;
		}
		virtual inline void setTrigger(bool isTrigger_) {
			using namespace physx;

			int nbShapes = actor->getNbShapes();
			if (nbShapes == 0) {
				throw NoShapeException{};
			}

			PxShape** shapes{};
			actor->getShapes(shapes, sizeof(PxShape) * nbShapes);

			for (int i = 0; i < nbShapes; i++) {
				shapes[i]->setFlag(PxShapeFlag::eTRIGGER_SHAPE, isTrigger_);
			}

			trigger = isTrigger_;
		}
	};
}