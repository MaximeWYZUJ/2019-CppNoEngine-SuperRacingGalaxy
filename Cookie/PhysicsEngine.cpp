#include "pch.h"
#include "PhysicsEngine.h"
#include "PhysicsContactCallback.h"
#include "PxPhysicsAPI.h"
#include <algorithm>

using namespace physx;

#define PVD_HOST "127.0.0.1"
#define PX_RELEASE(x)	if(x)	{ x->release(); x = nullptr;	}


PxFilterFlags filterShader(
	PxFilterObjectAttributes attributes0, PxFilterData filterData0,
	PxFilterObjectAttributes attributes1, PxFilterData filterData1,
	PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
	pairFlags = PxPairFlag::eCONTACT_DEFAULT;

	if ((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
	{
		pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND;
		pairFlags |= PxPairFlag::eMODIFY_CONTACTS;
		pairFlags |= PxPairFlag::eNOTIFY_TOUCH_LOST;
	}

	/*if ((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1)) {
		return PxFilterFlag::eKILL;
	}*/

	return PxFilterFlag::eDEFAULT;
}


namespace Cookie
{
	PhysicsEngine::~PhysicsEngine()
	{
		clean();
	}

	void PhysicsEngine::init()
	{
		static PxDefaultAllocator gAllocator{};
		static PxDefaultErrorCallback gErrorCallback{};
		
		gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);

		gPvd = PxCreatePvd(*gFoundation);
		PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
		gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

		gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, gPvd);

		PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
		sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
		gDispatcher = PxDefaultCpuDispatcherCreate(2);
		sceneDesc.cpuDispatcher = gDispatcher;

		PhysicsContactCallback* contactCb = new PhysicsContactCallback();
		sceneDesc.simulationEventCallback = contactCb;
		sceneDesc.contactModifyCallback = contactCb;

		sceneDesc.filterShader = filterShader;

		gScene = gPhysics->createScene(sceneDesc);

		PxPvdSceneClient* pvdClient = gScene->getScenePvdClient();
		if (pvdClient)
		{
			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
		}
	}

	void PhysicsEngine::step()
	{
		gScene->simulate(1.0f / 60.0f);
		gScene->fetchResults(true);
	}

	void PhysicsEngine::clean()
	{
		PX_RELEASE(gScene);
		PX_RELEASE(gDispatcher);
		PX_RELEASE(gPhysics);
		if (gPvd)
		{
			PxPvdTransport* transport = gPvd->getTransport();
			gPvd->release();
			gPvd = nullptr;
			PX_RELEASE(transport);
		}
		PX_RELEASE(gFoundation);
	}
	void PhysicsEngine::UpdateBoxActor(PhysicsBoxComponent* modifs, ActorPtr actor)
	{
		// ON NE PEUT PAS CHANGER LE BODY TYPE POUR LE MOMENT

		Vector3<PhysicsComponent::PhysicsComponent_t> modP = modifs->transform.GetPosition();
		Quaternion<PhysicsComponent::PhysicsComponent_t> modR = modifs->transform.GetRotation();
		actor->setGlobalPose(PxTransform(PxVec3(-modP.x, modP.y, modP.z), PxQuat(modR.x, modR.y, modR.z, modR.w)));

		if (modifs->type == PhysicsComponent::BodyType::DYNAMIC) {
			actor->is<PxRigidDynamic>()->setMass(modifs->mass);
			actor->is<PxRigidDynamic>()->setCMassLocalPose(PxTransform(PxVec3(-modifs->massCenter.x, modifs->massCenter.y, modifs->massCenter.z)));
			actor->is<PxRigidDynamic>()->setLinearVelocity(PxVec3(modifs->velocity.x, modifs->velocity.y, modifs->velocity.z));
		}

		// On calcule les nouveaux word0 et word1
		PxFilterData filter{};
		std::for_each(modifs->selfGroup.begin(), modifs->selfGroup.end(), [&filter](FilterGroup f) {
			filter.word0 |= f;
		});
		std::for_each(modifs->mask.begin(), modifs->mask.end(), [&filter](FilterGroup f) {
			filter.word1 |= f;
		});

		// Calcul du nouveau material
		PxMaterial* mat = PhysicsEngine::getInstance().gPhysics->createMaterial(modifs->material.staticFriction, modifs->material.dynamicFriction, modifs->material.bounce);

		// Parcourt de toutes les shapes
		int nbShapes = actor->getNbShapes();
		PxShape** shapes = new PxShape * [nbShapes];
		actor->getShapes(shapes, sizeof(PxShape) * nbShapes);

		for (int i = 0; i < nbShapes; i++) {
			shapes[i]->setFlag(PxShapeFlag::eTRIGGER_SHAPE, modifs->trigger);
			shapes[i]->setMaterials(&mat, 1);
			shapes[i]->setSimulationFilterData(filter);
		}

		// On met les data dans le userData de l'actor, utile pour les callbacks
		actor->userData = modifs;
	}
}