#pragma once

#include "incopiable.h"
#include "ExportMacro.h"

namespace physx {
	class PxDefaultAllocator;
	class PxDefaultErrorCallback;
	class PxDefaultCpuDispatcher;
	class PxFoundation;
	class PxPvd;
	class PxPhysics;
	class PxScene;
}
namespace Cookie {
	class COOKIE_API PhysicEngine : Incopiable
	{
	private:
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
