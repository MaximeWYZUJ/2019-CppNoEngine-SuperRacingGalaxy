#pragma once

#include "PxPhysicsAPI.h"
#include "incopiable.h"

namespace Cookie {
	class PhysicEngine : Incopiable
	{
	public:
		physx::PxPhysics* gPhysics = NULL;
		physx::PxFoundation* gFoundation = NULL;
		physx::PxScene* gScene = NULL;

		void init() {};
		void step() {};
		void clean() {};

		static PhysicEngine& getInstance() {
			static PhysicEngine instance;
			return instance;
		};
	};
}
