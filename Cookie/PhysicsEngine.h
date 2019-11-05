#pragma once

#include "incopiable.h"
#include "ExportMacro.h"
#include "PhysicsBoxComponent.h"
#include "PhysicsComponent.h"

namespace physx {
	class PxDefaultAllocator;
	class PxDefaultErrorCallback;
	class PxDefaultCpuDispatcher;
	class PxFoundation;
	class PxPvd;
	class PxPhysics;
	class PxScene;
	class PxActor;
}

namespace Cookie
{
	class COOKIE_API PhysicsEngine : Incopiable
	{
	public:
		using ActorPtr = physx::PxRigidActor*;

		~PhysicsEngine();
		physx::PxPhysics* gPhysics = nullptr;
		physx::PxFoundation* gFoundation = nullptr;
		physx::PxScene* gScene = nullptr;
		physx::PxPvd* gPvd = nullptr;

		// Boucles principales du moteur
		void init();
		void step();
		void clean();

		// Acces au singleton
		static PhysicsEngine& getInstance()
		{
			static PhysicsEngine* instance = new PhysicsEngine();
			return *instance;
		};

		// Creation/Suppression d'objets dans la scene
		void RemoveActor(ActorPtr actor);
		ActorPtr CreateBox(const PhysicsBoxComponent* box);
		
		// Modifications d'acteurs de la scene
		void UpdateBoxActor(PhysicsBoxComponent* modifs, ActorPtr toBeModified);

		// Modifications de composant
		void UpdateBoxComponent(ActorPtr actor, PhysicsBoxComponent* toBeModified);

	private:
		physx::PxDefaultCpuDispatcher* gDispatcher = nullptr;
	};
}
