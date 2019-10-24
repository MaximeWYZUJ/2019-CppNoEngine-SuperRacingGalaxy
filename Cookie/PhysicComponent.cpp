#include "pch.h"

#include <algorithm>

#include "PhysicComponent.h"
#include "PxPhysicsAPI.h"
#include "PhysicEngine.h"

using namespace physx;

namespace Cookie
{
	void PhysicComponent::addForce(Vector3<PhysicComponent_t> force)
	{
		if (type == STATIC)
			return;
		
		actor->is<PxRigidDynamic>()->addForce(PxVec3(-force.x, force.y, force.z));
	}
	
	void PhysicComponent::addFilterGroup(FilterGroup f)
	{
		if (std::find(selfGroup.begin(), selfGroup.end(), f) == selfGroup.end())
			selfGroup.push_back(f);
	}

	void PhysicComponent::removeFilterGroup(FilterGroup f)
	{
		if (auto it = std::find(selfGroup.begin(), selfGroup.end(), f); it != selfGroup.end())
			selfGroup.erase(it);
	}

	void PhysicComponent::addFilterMask(FilterGroup f)
	{
		if (std::find(mask.begin(), mask.end(), f) == mask.end())
			mask.push_back(f);
	}

	void PhysicComponent::removeFilterMask(FilterGroup f)
	{
		if (auto it = std::find(mask.begin(), mask.end(), f); it != mask.end())
			mask.erase(it);
	}
	
	void PhysicComponent::updateFilters()
	{
		int nbShapes = actor->getNbShapes();
		if (nbShapes == 0) {
			throw NoShapeException{};
		}

		PxShape** shapes{};
		actor->getShapes(shapes, sizeof(PxShape) * nbShapes);


		PxFilterData filterData;
		std::for_each(selfGroup.begin(), selfGroup.end(), [&filterData](FilterGroup f) {
			filterData.word0 |= f;
		});
		std::for_each(mask.begin(), mask.end(), [&filterData](FilterGroup f) {
			filterData.word1 |= f;
		});

		for (int i = 0; i < nbShapes; i++) {
			shapes[i]->setSimulationFilterData(filterData);
		}
	}

	Transform<PhysicComponent::PhysicComponent_t> PhysicComponent::getTransform() const noexcept {
		using namespace physx;

		PxTransform pxT = actor->getGlobalPose();
		Transform<PhysicComponent_t> t{};
		t.SetPosition(Vector3<PhysicComponent_t>(-pxT.p.x, pxT.p.y, pxT.p.z));
		t.SetRotation(Quaternion<PhysicComponent_t>(pxT.q.x, pxT.q.y, pxT.q.z, pxT.q.w));
		t.SetScale(Vector3<PhysicComponent_t>(1.0f, 1.0f, 1.0f));

		return t;
	}
	
	PhysicComponent::PhysicComponent_t PhysicComponent::getMass() const {
		using namespace physx;

		if (type == STATIC)
			throw NoMass_StaticObject_Exception{};

		return static_cast<PhysicComponent_t>(actor->is<PxRigidDynamic>()->getMass());
	}
	
	Vector3<PhysicComponent::PhysicComponent_t> PhysicComponent::getVelocity() const {
		using namespace physx;

		if (type == STATIC)
			throw NoVelocity_StaticObject_Exception{};

		PxVec3 velocity = actor->is<PxRigidDynamic>()->getLinearVelocity();
		return Vector3<PhysicComponent_t>(-velocity.x, velocity.y, velocity.z);
	}
	
	PhysicMaterial PhysicComponent::getMaterial() const noexcept {
		return material;
	}
	
	bool PhysicComponent::isTrigger() const noexcept {
		return trigger;
	}

	void PhysicComponent::setMass(PhysicComponent_t mass) {
		using namespace physx;

		if (type == STATIC)
			throw NoMass_StaticObject_Exception{};

		actor->is<PxRigidDynamic>()->setMass(static_cast<PxReal>(mass));
	}
	void PhysicComponent::setMaterial(PhysicMaterial material_) {
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
	
	void PhysicComponent::setTrigger(bool isTrigger_) {
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
}