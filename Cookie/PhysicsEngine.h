#pragma once

#include "incopiable.h"
#include "ExportMacro.h"
#include "PhysicsBoxComponent.h"
#include "PhysicsSphereComponent.h"
#include "PhysicsMeshComponent.h"
#include "PhysicsComponent.h"
#include "Mesh.h"

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
	class PxCooking;
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
		physx::PxCooking* cooking = nullptr;

		// Boucles principales du moteur
		void init();
		void step();
		void clean();

		/// <summary>
		///		Method to get the distance between a point and a planet. Should be a general method to get raycast results; currently specialized for our needs.
		/// </summary>
		std::pair<bool, float> PlanetRaycast(Vector3<> origin, Vector3<> unitDir, float distance);

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
		physx::PxShape* CreateMeshShape(PhysicsMeshComponent* meshComponent, physx::PxMaterial& mat);
		
		// Modifications d'acteurs de la scene
	public:
		void UpdateActor(PhysicsComponent* modifs, ActorPtr toBeModified);
	private:
		void UpdateBoxActor(PhysicsBoxComponent* modifs, ActorPtr toBeModified);
		void UpdateSphereActor(PhysicsSphereComponent* modifs, ActorPtr toBeModified);
		void UpdateMeshActor(PhysicsMeshComponent* modifs, ActorPtr toBeModified);

		// Modifications de composant
	public:
		void UpdateComponent(ActorPtr actor, PhysicsComponent* toBeModified);
	private:
		void UpdateBoxComponent(ActorPtr actor, PhysicsBoxComponent* toBeModified);
		void UpdateSphereComponent(ActorPtr actor, PhysicsSphereComponent* toBeModified);
		void UpdateMeshComponent(ActorPtr actor, PhysicsMeshComponent* toBeModified);

	private:
		PhysicsEngine();
		physx::PxDefaultCpuDispatcher* gDispatcher = nullptr;
	};
}
