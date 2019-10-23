#pragma once

#include "PxPhysicsAPI.h"
#include "incopiable.h"

namespace Cookie {
	class PhysicEngine : Incopiable
	{
	private:
		physx::PxDefaultAllocator gAllocator;
		physx::PxDefaultErrorCallback gErrorCallback;
		physx::PxDefaultCpuDispatcher* gDispatcher = nullptr;
	public:
		physx::PxPhysics* gPhysics = nullptr;
		physx::PxFoundation* gFoundation = nullptr;
		physx::PxScene* gScene = nullptr;
		physx::PxPvd* gPvd = nullptr;

		void init();
		void step();
		void clean();

		static PhysicEngine& getInstance() {
			static PhysicEngine instance;
			return instance;
		};
	};
}
