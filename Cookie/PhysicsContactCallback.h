#pragma once
#include "PxContactModifyCallback.h"
#include "PxSimulationEventCallback.h"

namespace Cookie {
	using physx::PxU32;

	class PhysicsContactCallback : public physx::PxContactModifyCallback, public physx::PxSimulationEventCallback
	{
	public:
		// H�rit� via PxContactModifyCallback
		virtual void onContactModify(physx::PxContactModifyPair* const pairs, PxU32 count) override;


		// H�rit� via PxSimulationEventCallback
		virtual void onConstraintBreak(physx::PxConstraintInfo* constraints, PxU32 count) override;

		virtual void onWake(physx::PxActor** actors, PxU32 count) override;

		virtual void onSleep(physx::PxActor** actors, PxU32 count) override;

		virtual void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, PxU32 nbPairs) override;

		virtual void onTrigger(physx::PxTriggerPair* pairs, PxU32 count) override;

		virtual void onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const PxU32 count) override;
	};

}