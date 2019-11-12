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
	PhysicsEngine::PhysicsEngine()
	{
		init();
	}
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
	
	void PhysicsEngine::RemoveActor(ActorPtr actor)
	{
		gScene->removeActor(*actor);
	}

	PhysicsEngine::ActorPtr PhysicsEngine::CreateBox(PhysicsBoxComponent* box)
	{
		Vector3<PhysicsComponent::PhysicsComponent_t> pos = box->transform.GetPosition();
		Quaternion<PhysicsComponent::PhysicsComponent_t> rot = box->transform.GetRotation();
		PxTransform transform(PxVec3(-pos.x, pos.y, pos.z), PxQuat(-rot.x, rot.y, rot.z, rot.w));
		
		PxMaterial* material = gPhysics->createMaterial(box->material.staticFriction, box->material.dynamicFriction, box->material.bounce);
		Vector3<> scaling = box->transform.GetScale();

		PxShape* shape = gPhysics->createShape(PxBoxGeometry(box->dx / 2 * scaling.x, box->dy / 2 * scaling.y, box->dz / 2 * scaling.z), *material);
		shape->setSimulationFilterData(PxFilterData(DEFAULT, DEFAULT, 0, 0));

		PxRigidActor* actor = nullptr;
		if (box->type == PhysicsComponent::DYNAMIC) {
			actor = gPhysics->createRigidDynamic(transform);
		}
		else {
			actor = gPhysics->createRigidStatic(transform);
		}

		PxFilterData filterNull{};
		shape->setSimulationFilterData(filterNull);
		actor->attachShape(*shape);

		actor->userData = box;
		gScene->addActor(*actor);

		return actor;
	}

	void PhysicsEngine::UpdateBoxActor(PhysicsBoxComponent* modifs, ActorPtr actor)
	{
		// ON NE PEUT PAS CHANGER LE BODY TYPE POUR LE MOMENT

		Vector3<PhysicsComponent::PhysicsComponent_t> modP = modifs->transform.GetPosition();
		Quaternion<PhysicsComponent::PhysicsComponent_t> modR = modifs->transform.GetRotation();
		actor->setGlobalPose(PxTransform(PxVec3(-modP.x, modP.y, modP.z), PxQuat(modR.x, modR.y, modR.z, modR.w)));

		if (modifs->type == PhysicsComponent::BodyType::DYNAMIC) {
			static_cast<PxRigidDynamic*>(actor)->setMass(modifs->mass);
			static_cast<PxRigidDynamic*>(actor)->setCMassLocalPose(PxTransform(PxVec3(-modifs->massCenter.x, modifs->massCenter.y, modifs->massCenter.z)));
			static_cast<PxRigidDynamic*>(actor)->setLinearVelocity(PxVec3(modifs->velocity.x, modifs->velocity.y, modifs->velocity.z));
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
		actor->getShapes(shapes, sizeof(PxShape*) * nbShapes);

		for (int i = 0; i < nbShapes; i++) {
			shapes[i]->setFlag(PxShapeFlag::eTRIGGER_SHAPE, modifs->trigger);
			shapes[i]->setMaterials(&mat, 1);
			shapes[i]->setSimulationFilterData(filter);
		}

		// On met les data dans le userData de l'actor, utile pour les callbacks
		actor->userData = modifs;
	}
	
	void PhysicsEngine::UpdateBoxComponent(ActorPtr actor, PhysicsBoxComponent* toBeModified)
	{
		PxVec3 pos = actor->getGlobalPose().p;
		toBeModified->transform.SetPosition(Vector3<PhysicsComponent::PhysicsComponent_t>(-pos.x, pos.y, pos.z));
		PxQuat rot = actor->getGlobalPose().q;
		toBeModified->transform.SetRotation(Quaternion<PhysicsComponent::PhysicsComponent_t>(-rot.x, rot.y, rot.z, rot.w));
		toBeModified->transform.SetScale(toBeModified->parent->localTransform.GetScale());

		if (toBeModified->type == PhysicsComponent::DYNAMIC) {
			PxVec3 velo = static_cast<PxRigidDynamic*>(actor)->getLinearVelocity();
			toBeModified->velocity = Vector3<PhysicsComponent::PhysicsComponent_t>(-velo.x, velo.y, velo.z);

			toBeModified->mass = static_cast<PxRigidDynamic*>(actor)->getMass();
			PxVec3 cmass = static_cast<PxRigidDynamic*>(actor)->getCMassLocalPose().p;
			toBeModified->massCenter = Vector3<PhysicsComponent::PhysicsComponent_t>(-cmass.x, cmass.y, cmass.z);
		}

		int nbShapes = actor->getNbShapes();
		PxShape** shapes = new PxShape * [nbShapes];
		actor->getShapes(shapes, sizeof(PxShape*) * nbShapes);
		PxShape* shape = shapes[0];

		PxMaterial** materials = new PxMaterial * [1];
		shape->getMaterials(materials, sizeof(PxMaterial*));

		toBeModified->trigger = shape->getFlags().isSet(PxShapeFlag::eTRIGGER_SHAPE);
		toBeModified->material = PhysicMaterial(materials[0]->getRestitution(), materials[0]->getStaticFriction(), materials[0]->getDynamicFriction());

		toBeModified->actor = actor;
	}
}