#include "pch.h"
#include "PhysicsContactCallback.h"
#include "PxRigidActor.h"
#include "PhysicsComponent.h"

using namespace physx;

namespace Cookie {

	void PhysicsContactCallback::onContactModify(PxContactModifyPair* const pairs, PxU32 count)
	{
		for (int i = 0; i < count; i++) {
			if (pairs[i].shape[0]->getSimulationFilterData().word1 & pairs[i].shape[1]->getSimulationFilterData().word0) {
				PhysicsComponent* self = static_cast<PhysicsComponent*>(pairs[i].actor[0]->userData);
				PhysicsComponent* other = static_cast<PhysicsComponent*>(pairs[i].actor[1]->userData);
				(*self->onCollisionCallback)(other);
			}

			if (pairs[i].shape[1]->getSimulationFilterData().word1 & pairs[i].shape[0]->getSimulationFilterData().word0) {
				PhysicsComponent* self = static_cast<PhysicsComponent*>(pairs[i].actor[1]->userData);
				PhysicsComponent* other = static_cast<PhysicsComponent*>(pairs[i].actor[0]->userData);
				(*self->onCollisionCallback)(other);
			}
		}
	}

	void PhysicsContactCallback::onTrigger(PxTriggerPair* pairs, PxU32 count)
	{
		for (int i = 0; i < count; i++) {
			if (pairs[i].triggerShape->getSimulationFilterData().word1 & pairs[i].otherShape->getSimulationFilterData().word0) {
				PhysicsComponent* self = static_cast<PhysicsComponent*>(pairs[i].triggerActor->userData);
				PhysicsComponent* other = static_cast<PhysicsComponent*>(pairs[i].otherActor->userData);
				(*self->onCollisionCallback)(other);
			}
		}
	}

	void PhysicsContactCallback::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) {}

	void PhysicsContactCallback::onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) {}

	void PhysicsContactCallback::onWake(PxActor** actors, PxU32 count) {}

	void PhysicsContactCallback::onSleep(PxActor** actors, PxU32 count) {}

	void PhysicsContactCallback::onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count) {}

}