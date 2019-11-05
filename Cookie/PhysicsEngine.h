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

namespace Cookie
{
	class COOKIE_API PhysicsEngine : Incopiable
	{
	public:
		~PhysicsEngine();
		physx::PxPhysics* gPhysics = nullptr;
		physx::PxFoundation* gFoundation = nullptr;
		physx::PxScene* gScene = nullptr;
		physx::PxPvd* gPvd = nullptr;

		void init();
		void step();
		void clean();

		static PhysicsEngine& getInstance()
		{
			static PhysicsEngine* instance = new PhysicsEngine();
			return *instance;
		};
	private:
		physx::PxDefaultCpuDispatcher* gDispatcher = nullptr;
	};
}
