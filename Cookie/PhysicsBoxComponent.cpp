#include "pch.h"
#include "PhysicsBoxComponent.h"
#include "PhysicsEngine.h"
#include "PxPhysicsAPI.h"

using namespace physx;

namespace Cookie {
	PhysicsBoxComponent::PhysicsBoxComponent(Vector3<PhysicsComponent_t> pos, Quaternion<PhysicsComponent_t> rot, PhysicMaterial mat, BodyType type, float _dx, float _dy, float _dz)
					  : dx(_dx), dy(_dy), dz(_dz)
	{
		auto& engine= PhysicsEngine::getInstance();
		PxTransform transform(PxVec3(-pos.x, pos.y, pos.z), PxQuat(rot.x, rot.y, rot.z, rot.w));
		PxMaterial* material = engine.gPhysics->createMaterial(mat.staticFriction, mat.dynamicFriction, mat.bounce);

		PxShape* shape = engine.gPhysics->createShape(PxBoxGeometry(dx / 2, dy / 2, dz / 2), *material);
		shape->setSimulationFilterData(PxFilterData(DEFAULT, DEFAULT, 0, 0));

		if (type == DYNAMIC) {
			actor = engine.gPhysics->createRigidDynamic(transform);
			//actor = PxCreateDynamic(*engine.gPhysics, transform, PxBoxGeometry(dx / 2, dy / 2, dz / 2), *material, 1.0f);
		}
		else {
			actor = engine.gPhysics->createRigidStatic(transform);
			//actor = PxCreateStatic(*engine.gPhysics, transform, PxBoxGeometry(dx / 2, dy / 2, dz / 2), *material);
		}

		PxFilterData filterNull{};
		shape->setSimulationFilterData(filterNull);
		actor->attachShape(*shape);

		actor->userData = this;
		engine.gScene->addActor(*actor);
	}
}
