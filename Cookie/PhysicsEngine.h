#pragma once

#include "incopiable.h"
#include "ExportMacro.h"
#include "PhysicsBoxComponent.h"
#include "PhysicsSphereComponent.h"
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
	class PxShape;
	class PxMaterial;
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
		ActorPtr CreateComponent(PhysicsComponent* component);
	private:
		physx::PxShape* CreateBoxShape(PhysicsBoxComponent* box, physx::PxMaterial& mat);
		physx::PxShape* CreateSphereShape(PhysicsSphereComponent* sphere, physx::PxMaterial& mat);
		
		// Modifications d'acteurs de la scene
	public:
		void UpdateActor(PhysicsComponent* modifs, ActorPtr toBeModified);
	private:
		void UpdateBoxActor(PhysicsBoxComponent* modifs, ActorPtr toBeModified);
		void UpdateSphereActor(PhysicsSphereComponent* modifs, ActorPtr toBeModified);

		// Modifications de composant
	public:
		void UpdateComponent(ActorPtr actor, PhysicsComponent* toBeModified);
	private:
		void UpdateBoxComponent(ActorPtr actor, PhysicsBoxComponent* toBeModified);
		void UpdateSphereComponent(ActorPtr actor, PhysicsSphereComponent* toBeModified);

	private:
		PhysicsEngine();
		physx::PxDefaultCpuDispatcher* gDispatcher = nullptr;
	};
}
