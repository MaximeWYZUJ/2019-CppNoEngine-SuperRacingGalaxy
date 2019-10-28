#include "pch.h"
#include "PhysicContactCallback.h"
#include "PxRigidActor.h"
#include "PhysicComponent.h"
#include <fstream>

using namespace physx;

namespace Cookie {

	void PhysicContactCallback::onContactModify(PxContactModifyPair* const pairs, PxU32 count)
	{
		std::fstream file("output.txt", std::ofstream::app);
		file << "on contact modify" << std::endl;

		for (int i = 0; i < count; i++) {
			if (pairs[i].shape[0]->getSimulationFilterData().word1 & pairs[i].shape[1]->getSimulationFilterData().word0) {
				PhysicComponent* self = static_cast<PhysicComponent*>(pairs[i].actor[0]->userData);
				PhysicComponent* other = static_cast<PhysicComponent*>(pairs[i].actor[1]->userData);
				self->onCollisionCallBack(other);
			}

			if (pairs[i].shape[1]->getSimulationFilterData().word1 & pairs[i].shape[0]->getSimulationFilterData().word0) {
				PhysicComponent* self = static_cast<PhysicComponent*>(pairs[i].actor[1]->userData);
				PhysicComponent* other = static_cast<PhysicComponent*>(pairs[i].actor[0]->userData);
				self->onCollisionCallBack(other);
			}
		}
	}

	void PhysicContactCallback::onTrigger(PxTriggerPair* pairs, PxU32 count)
	{
		for (int i = 0; i < count; i++) {
			if (pairs[i].triggerShape->getSimulationFilterData().word1 & pairs[i].otherShape->getSimulationFilterData().word0) {
				PhysicComponent* self = static_cast<PhysicComponent*>(pairs[i].triggerActor->userData);
				PhysicComponent* other = static_cast<PhysicComponent*>(pairs[i].otherActor->userData);
				self->onTriggerCallBack(other);
			}
		}
	}

	void PhysicContactCallback::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) {}

	void PhysicContactCallback::onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) {}

	void PhysicContactCallback::onWake(PxActor** actors, PxU32 count) {}

	void PhysicContactCallback::onSleep(PxActor** actors, PxU32 count) {}

	void PhysicContactCallback::onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count) {}

}