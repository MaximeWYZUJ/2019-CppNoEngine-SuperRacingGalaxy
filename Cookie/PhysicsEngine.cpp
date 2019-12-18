#include "pch.h"
#include "PhysicsEngine.h"
#include "PhysicsContactCallback.h"
#include "PxPhysicsAPI.h"
#include <vector>
#include "RaycastCallback.h"
#include <algorithm>

using namespace physx;

#define PVD_HOST "127.0.0.1"
#define PX_RELEASE(x)	if(x)	{ x->release(); x = nullptr;	}


PxFilterFlags filterShader(
	PxFilterObjectAttributes attributes0, PxFilterData filterData0,
	PxFilterObjectAttributes attributes1, PxFilterData filterData1,
	PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
	// Triggers
	if (PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
	{
		if ((filterData0.word0 & filterData1.word1) || (filterData1.word0 & filterData0.word1)) {
			pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
			return PxFilterFlag::eDEFAULT;
		}
	}

	// Non triggers
	pairFlags = PxPairFlag::eCONTACT_DEFAULT;
	if ((filterData0.word0 & filterData1.word1) || (filterData1.word0 & filterData0.word1))
	{
		pairFlags = PxPairFlag::eSOLVE_CONTACT;
		pairFlags |= PxPairFlag::eDETECT_DISCRETE_CONTACT;
		pairFlags |= PxPairFlag::eDETECT_CCD_CONTACT;
		pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND;
		pairFlags |= PxPairFlag::eMODIFY_CONTACTS;
		pairFlags |= PxPairFlag::eNOTIFY_TOUCH_LOST;
	}

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

		PxTolerancesScale tolScale;
		PxCookingParams params(tolScale);
		cooking = PxCreateCooking(PX_PHYSICS_VERSION, *gFoundation, params);

		PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
		sceneDesc.flags |= PxSceneFlag::eENABLE_CCD;
		sceneDesc.gravity = PxVec3(0.0f, 0.0f, 0.0f);
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

	std::pair<bool, float> PhysicsEngine::PlanetRaycast(Vector3<> origin, Vector3<> unitDir, float distance)
	{
		PxRaycastBuffer buf;
		RaycastCallback callback;
		
		auto r = gScene->raycast(
			PxVec3(origin.x, origin.y, origin.z),
			PxVec3(unitDir.x, unitDir.y, unitDir.z),
			distance, buf,
			PxHitFlags(PxHitFlag::eDEFAULT), PxQueryFilterData(), &callback);

		if (!r)
		{
			return { false, -1.0f };
		}

		PxVec3 p = buf.block.position;
		return { r, Vector3<>(p.x - origin.x, p.y - origin.y, p.z - origin.z).Length() };
	}
	
	void PhysicsEngine::RemoveActor(ActorPtr actor)
	{
		gScene->removeActor(*actor);
	}

	PhysicsEngine::ActorPtr PhysicsEngine::CreateComponent(PhysicsComponent* compo)
	{
		Vector3<PhysicsComponent::PhysicsComponent_t> pos = compo->transform.GetPosition();
		Quaternion<PhysicsComponent::PhysicsComponent_t> rot = compo->transform.GetRotation();
		PxTransform transform(PxVec3(pos.x, pos.y, pos.z), PxQuat(rot.x, rot.y, rot.z, rot.w));
		
		PxMaterial* material = gPhysics->createMaterial(compo->material.staticFriction, compo->material.dynamicFriction, compo->material.bounce);
		//Vector3<> scaling = box->transform.GetScale();

		// Shape
		PxShape* shape = nullptr;
		switch (compo->getShapeType()) {
		case PhysicsComponent::ShapeType::BOX:
			shape = CreateBoxShape(static_cast<PhysicsBoxComponent*>(compo), *material);
			break;

		case PhysicsComponent::ShapeType::SPHERE:
			shape = CreateSphereShape(static_cast<PhysicsSphereComponent*>(compo), *material);
			break;

		case PhysicsComponent::ShapeType::MESH:
			shape = CreateMeshShape(static_cast<PhysicsMeshComponent*>(compo), *material);
			break;

		default:
			break;
		}
		if (compo->trigger) {
			shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
			shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
		}

		// Simulation filter data
		PxFilterData filter{};
		std::for_each(compo->selfGroup.begin(), compo->selfGroup.end(), [&filter](FilterGroup f) {
			filter.word0 |= f;
		});
		std::for_each(compo->mask.begin(), compo->mask.end(), [&filter](FilterGroup f) {
			filter.word1 |= f;
		});
		shape->setSimulationFilterData(filter);

		// Body
		PxRigidActor* actor = nullptr;
		if (compo->type == PhysicsComponent::DYNAMIC) {
			PxRigidBody *x = gPhysics->createRigidDynamic(transform);
			x->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_CCD, true);
			actor = x;
		}
		else {
			actor = gPhysics->createRigidStatic(transform);
		}
		

		actor->attachShape(*shape);

		actor->userData = compo;
		gScene->addActor(*actor);

		shape->release();

		return actor;
	}

	PxShape* PhysicsEngine::CreateBoxShape(PhysicsBoxComponent* box, PxMaterial& mat)
	{
		Vector3<> scaling = box->transform.GetScale();
		return gPhysics->createShape(PxBoxGeometry(box->dx / 2 * scaling.x, box->dy / 2 * scaling.y, box->dz / 2 * scaling.z), mat);
	}

	PxShape* PhysicsEngine::CreateSphereShape(PhysicsSphereComponent* sphere, PxMaterial& mat)
	{
		return gPhysics->createShape(PxSphereGeometry(sphere->transform.GetScale().x * sphere->radius / 2), mat);
	}

	physx::PxShape* PhysicsEngine::CreateMeshShape(PhysicsMeshComponent* meshComponent, physx::PxMaterial& mat)
	{
		Mesh& mesh = meshComponent->mesh;

		PxTriangleMeshDesc meshDesc;
		meshDesc.points.count = mesh.GetVerticesPx().size();
		meshDesc.points.stride = sizeof(mesh.GetVerticesPx().at(0));
		meshDesc.points.data = mesh.GetVerticesPx().data();

		meshDesc.triangles.count = mesh.GetTrianglesPx().size();
		meshDesc.triangles.stride = sizeof(mesh.GetTrianglesPx().at(0));
		meshDesc.triangles.data = mesh.GetTrianglesPx().data();

		PxDefaultMemoryOutputStream writeBuffer;
		PxTriangleMeshCookingResult::Enum* result = nullptr;
		bool status = cooking->cookTriangleMesh(meshDesc, writeBuffer, result);
		if (!status)
			return nullptr;

		PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
		PxTriangleMesh* triangleMesh = gPhysics->createTriangleMesh(readBuffer);

		Vector3<> scale = meshComponent->transform.GetScale();
		PxMeshScale meshScale(PxVec3(scale.x, scale.y, scale.z));
		PxTriangleMeshGeometry geom(triangleMesh, meshScale);

		return gPhysics->createShape(geom, mat, true);
	}

	void PhysicsEngine::UpdateActor(PhysicsComponent* modifs, ActorPtr actor)
	{
		// ON NE PEUT PAS CHANGER LE BODY TYPE POUR LE MOMENT
		/*
		switch (modifs->getShapeType()) {
		case PhysicsComponent::ShapeType::BOX:
			UpdateBoxActor(static_cast<PhysicsBoxComponent*>(modifs), actor);
			break;

		case PhysicsComponent::ShapeType::SPHERE:
			UpdateSphereActor(static_cast<PhysicsSphereComponent*>(modifs), actor);
			break;

		case PhysicsComponent::ShapeType::MESH:
			UpdateMeshActor(static_cast<PhysicsMeshComponent*>(modifs), actor);
			break;

		default:
			break;
		}
		*/

		actor->is<physx::PxRigidDynamic>()->setAngularVelocity(physx::PxVec3(modifs->velocityAngular.x, modifs->velocityAngular.y, modifs->velocityAngular.z));

		Vector3<PhysicsComponent::PhysicsComponent_t> modP = modifs->transform.GetPosition();
		Quaternion<PhysicsComponent::PhysicsComponent_t> modR = modifs->transform.GetRotation();
		actor->setGlobalPose(PxTransform(PxVec3(modP.x, modP.y, modP.z), PxQuat(modR.x, modR.y, modR.z, modR.w)));

		if (modifs->type == PhysicsComponent::BodyType::DYNAMIC) {
			static_cast<PxRigidDynamic*>(actor)->setMass(modifs->mass);
			static_cast<PxRigidDynamic*>(actor)->setCMassLocalPose(PxTransform(PxVec3(-modifs->massCenter.x, modifs->massCenter.y, modifs->massCenter.z)));
			static_cast<PxRigidDynamic*>(actor)->setLinearVelocity(PxVec3(modifs->velocity.x, modifs->velocity.y, modifs->velocity.z));

			if (modifs->resetAcceleration) {
				static_cast<PxRigidDynamic*>(actor)->clearForce();
				static_cast<PxRigidDynamic*>(actor)->clearTorque();
				static_cast<PxRigidDynamic*>(actor)->setLinearVelocity(PxVec3(0, 0, 0));

				modifs->resetAcceleration = false;
			}
		}

		std::for_each(modifs->addedForces.begin(), modifs->addedForces.end(), [&actor](Vector3<PhysicsComponent::PhysicsComponent_t> force)
		{
			actor->is<physx::PxRigidDynamic>()->addForce(physx::PxVec3(force.x, force.y, force.z));
		});
		modifs->addedForces.clear();

		std::for_each(modifs->addedImpulses.begin(), modifs->addedImpulses.end(), [&actor](Vector3<PhysicsComponent::PhysicsComponent_t> impulse) {
			actor->is<physx::PxRigidDynamic>()->addForce(physx::PxVec3(impulse.x, impulse.y, impulse.z), physx::PxForceMode::eIMPULSE);
		});
		modifs->addedImpulses.clear();

		// On calcule les nouveaux word0 et word1
		/*PxFilterData filter{};
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
		}*/

		// On met les data dans le userData de l'actor, utile pour les callbacks
		//actor->userData = modifs;
	}

	void PhysicsEngine::UpdateBoxActor(PhysicsBoxComponent* modifs, ActorPtr toBeModified)
	{
		Vector3<> scaling = modifs->transform.GetScale();

		int nbShapes = toBeModified->getNbShapes();
		PxShape** shapes = new PxShape * [nbShapes];
		toBeModified->getShapes(shapes, sizeof(PxShape*) * nbShapes);

		for (int i = 0; i < nbShapes; i++) {
			shapes[i]->setGeometry(PxBoxGeometry(modifs->dx / 2 * scaling.x, modifs->dy / 2 * scaling.y, modifs->dz / 2 * scaling.z));
		}
	}

	void PhysicsEngine::UpdateSphereActor(PhysicsSphereComponent* modifs, ActorPtr toBeModified)
	{
		Vector3<> scaling = modifs->transform.GetScale();

		int nbShapes = toBeModified->getNbShapes();
		PxShape** shapes = new PxShape * [nbShapes];
		toBeModified->getShapes(shapes, sizeof(PxShape*) * nbShapes);

		for (int i = 0; i < nbShapes; i++) {
			shapes[i]->setGeometry(PxSphereGeometry(scaling.x * modifs->radius));
		}
	}

	void PhysicsEngine::UpdateMeshActor(PhysicsMeshComponent* modifs, ActorPtr toBeModified)
	{
		// CHANGER LE MESH
	}
	
	void PhysicsEngine::UpdateComponent(ActorPtr actor, PhysicsComponent* toBeModified)
	{
		switch (toBeModified->getShapeType()) {
		case PhysicsComponent::ShapeType::BOX:
			UpdateBoxComponent(actor, static_cast<PhysicsBoxComponent*>(toBeModified));
			break;

		case PhysicsComponent::ShapeType::SPHERE:
			UpdateSphereComponent(actor, static_cast<PhysicsSphereComponent*>(toBeModified));
			break;

		case PhysicsComponent::ShapeType::MESH:
			UpdateMeshComponent(actor, static_cast<PhysicsMeshComponent*>(toBeModified));
			break;

		default:
			break;
		}

		PxVec3 pos = actor->getGlobalPose().p;
		toBeModified->transform.SetPosition(Vector3<PhysicsComponent::PhysicsComponent_t>(pos.x, pos.y, pos.z));
		PxQuat rot = actor->getGlobalPose().q;
		toBeModified->transform.SetRotation(Quaternion<PhysicsComponent::PhysicsComponent_t>(rot.x, rot.y, rot.z, rot.w));
		toBeModified->transform.SetScale(toBeModified->sceneNode->localTransform.GetScale());

		if (toBeModified->type == PhysicsComponent::DYNAMIC) {
			PxVec3 velo = static_cast<PxRigidDynamic*>(actor)->getLinearVelocity();
			toBeModified->velocity = Vector3<PhysicsComponent::PhysicsComponent_t>(velo.x, velo.y, velo.z);

			PxVec3 angVelo = static_cast<PxRigidDynamic*>(actor)->getAngularVelocity();
			toBeModified->velocityAngular = Vector3<PhysicsComponent::PhysicsComponent_t>(angVelo.x, angVelo.y, angVelo.z);

			toBeModified->mass = static_cast<PxRigidDynamic*>(actor)->getMass();
			PxVec3 cmass = static_cast<PxRigidDynamic*>(actor)->getCMassLocalPose().p;
			toBeModified->massCenter = Vector3<PhysicsComponent::PhysicsComponent_t>(cmass.x, cmass.y, cmass.z);
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

	void PhysicsEngine::UpdateBoxComponent(ActorPtr actor, PhysicsBoxComponent* toBeModified)
	{
		int nbShapes = actor->getNbShapes();
		PxShape** shapes = new PxShape * [nbShapes];
		actor->getShapes(shapes, sizeof(PxShape*) * nbShapes);

		PxBoxGeometry geom;
		shapes[0]->getBoxGeometry(geom);
		
		toBeModified->dx = (geom.halfExtents.x / toBeModified->transform.GetScale().x) * 2;
		toBeModified->dy = (geom.halfExtents.y / toBeModified->transform.GetScale().y) * 2;
		toBeModified->dz = (geom.halfExtents.z / toBeModified->transform.GetScale().z) * 2;
	}
	void PhysicsEngine::UpdateSphereComponent(ActorPtr actor, PhysicsSphereComponent* toBeModified)
	{
		int nbShapes = actor->getNbShapes();
		PxShape** shapes = new PxShape * [nbShapes];
		actor->getShapes(shapes, sizeof(PxShape*) * nbShapes);

		PxSphereGeometry geom;
		shapes[0]->getSphereGeometry(geom);

		toBeModified->radius = geom.radius / toBeModified->transform.GetScale().x;
	}
	void PhysicsEngine::UpdateMeshComponent(ActorPtr actor, PhysicsMeshComponent* toBeModified)
	{
		// MAJ LE MESH A PARTIR DE LA PHYSIQUE
	}
}