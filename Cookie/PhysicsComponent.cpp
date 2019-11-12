#include "pch.h"

#include <algorithm>

#include "PhysicsComponent.h"
#include "PxPhysicsAPI.h"
#include "PhysicsEngine.h"
#include "PxFiltering.h"

using namespace physx;

namespace Cookie
{
	void PhysicsComponent::addForce(Vector3<PhysicsComponent_t> force)
	{
		if (type == STATIC)
			return;
		
		addedForces.push_back(force);
		actor->is<PxRigidDynamic>()->addForce(PxVec3(-force.x, force.y, force.z));
	}
	
	void PhysicsComponent::addFilterGroup(FilterGroup f)
	{
		selfGroup.insert(f);
	}

	void PhysicsComponent::removeFilterGroup(FilterGroup f)
	{
		if (auto it = std::find(selfGroup.begin(), selfGroup.end(), f); it != selfGroup.end())
			selfGroup.erase(it);
	}

	void PhysicsComponent::addFilterMask(FilterGroup f)
	{
		mask.insert(f);
	}

	void PhysicsComponent::removeFilterMask(FilterGroup f)
	{
		if (auto it = std::find(mask.begin(), mask.end(), f); it != mask.end())
			mask.erase(it);
	}
	
	/*void PhysicsComponent::updateFilters()
	{
		int nbShapes = actor->getNbShapes();
		if (nbShapes == 0) {
			throw NoShapeException{};
		}

		PxShape** shapes{};
		actor->getShapes(shapes, sizeof(PxShape) * nbShapes);


		PxFilterData filter{};
		std::for_each(selfGroup.begin(), selfGroup.end(), [&filter](FilterGroup f) {
			filter.word0 |= f;
		});
		std::for_each(mask.begin(), mask.end(), [&filter](FilterGroup f) {
			filter.word1 |= f;
		});

		for (int i = 0; i < nbShapes; i++) {
			shapes[i]->setSimulationFilterData(filter);
		}
	}

	Transform<PhysicsComponent::PhysicsComponent_t> PhysicsComponent::getTransform() const noexcept {
		using namespace physx;

		PxTransform pxT = actor->getGlobalPose();
		Transform<PhysicsComponent_t> t{};
		t.SetPosition(Vector3<PhysicsComponent_t>(-pxT.p.x, pxT.p.y, pxT.p.z));
		t.SetRotation(Quaternion<PhysicsComponent_t>(pxT.q.x, pxT.q.y, pxT.q.z, pxT.q.w));
		t.SetScale(Vector3<PhysicsComponent_t>(1.0f, 1.0f, 1.0f));

		return t;
	}
	
	PhysicsComponent::PhysicsComponent_t PhysicsComponent::getMass() const {
		using namespace physx;

		if (type == STATIC)
			throw NoMass_StaticObject_Exception{};

		return static_cast<PhysicsComponent_t>(actor->is<PxRigidDynamic>()->getMass());
	}
	
	Vector3<PhysicsComponent::PhysicsComponent_t> PhysicsComponent::getVelocity() const {
		using namespace physx;

		if (type == STATIC)
			throw NoVelocity_StaticObject_Exception{};

		PxVec3 velocity = actor->is<PxRigidDynamic>()->getLinearVelocity();
		return Vector3<PhysicsComponent_t>(-velocity.x, velocity.y, velocity.z);
	}
	
	PhysicMaterial PhysicsComponent::getMaterial() const noexcept {
		return material;
	}
	
	bool PhysicsComponent::isTrigger() const noexcept {
		return trigger;
	}

	void PhysicsComponent::setMass(PhysicsComponent_t mass) {
		using namespace physx;

		if (type == STATIC)
			throw NoMass_StaticObject_Exception{};

		actor->is<PxRigidDynamic>()->setMass(static_cast<PxReal>(mass));
	}
	void PhysicsComponent::setMaterial(PhysicMaterial material_) {
		using namespace physx;

		int nbShapes = actor->getNbShapes();
		if (nbShapes == 0) {
			throw NoShapeException{};
		}

		PxShape** shapes{};
		actor->getShapes(shapes, sizeof(PxShape) * nbShapes);

		PxMaterial* mat = PhysicsEngine::getInstance().gPhysics->createMaterial(material_.staticFriction, material_.dynamicFriction, material_.bounce);
		for (int i = 0; i < nbShapes; i++) {
			shapes[i]->setMaterials(&mat, 1);
		}

		material = material_;
	}
	
	void PhysicsComponent::setTrigger(bool isTrigger_) {
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
	}*/
}