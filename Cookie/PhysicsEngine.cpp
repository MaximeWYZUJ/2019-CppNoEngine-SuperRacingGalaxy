#include "pch.h"
#include "PhysicsEngine.h"
#include "PhysicsContactCallback.h"
#include "PxPhysicsAPI.h"

using namespace physx;

#define PVD_HOST "127.0.0.1"
#define PX_RELEASE(x)	if(x)	{ x->release(); x = nullptr;	}


PxFilterFlags filterShader(
	PxFilterObjectAttributes attributes0, PxFilterData filterData0,
	PxFilterObjectAttributes attributes1, PxFilterData filterData1,
	PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
	pairFlags = PxPairFlag::eCONTACT_DEFAULT;

	if ((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1)) {
		pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND;
		pairFlags |= PxPairFlag::eMODIFY_CONTACTS;
		pairFlags |= PxPairFlag::eNOTIFY_TOUCH_LOST;
	}

	/*if ((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1)) {
		return PxFilterFlag::eKILL;
	}*/

	return PxFilterFlag::eDEFAULT;
}


namespace Cookie {

	void PhysicsEngine::init()
	{
		static PxDefaultAllocator gAllocator{};
		static PxDefaultErrorCallback gErrorCallback{};
		
		// Necessaire pour le setup
		gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);

		// PVD = Physx Visual Debugger
		gPvd = PxCreatePvd(*gFoundation);
		PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
		gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

		gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, gPvd);

		// Descripteur de la scene
		PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
		sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
		gDispatcher = PxDefaultCpuDispatcherCreate(2);
		sceneDesc.cpuDispatcher = gDispatcher;

		PhysicsContactCallback* contactCb = new PhysicsContactCallback();
		sceneDesc.simulationEventCallback = contactCb;
		sceneDesc.contactModifyCallback = contactCb;

		sceneDesc.filterShader = filterShader;

		gScene = gPhysics->createScene(sceneDesc);


		// Pour le debugger
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
}