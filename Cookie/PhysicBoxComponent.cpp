#include "pch.h"
#include "PhysicBoxComponent.h"
#include "PhysicEngine.h"
#include "PxPhysicsAPI.h"

using namespace physx;

namespace Cookie {
	PhysicBoxComponent::PhysicBoxComponent(Vector3<PhysicComponent_t> pos, Quaternion<PhysicComponent_t> rot, PhysicMaterial mat, bodyType type, float _dx, float _dy, float _dz)
					  : dx(_dx), dy(_dy), dz(_dz)
	{
		auto& engine= PhysicEngine::getInstance();
		PxTransform transform(PxVec3(-pos.x, pos.y, pos.z), PxQuat(rot.x, rot.y, rot.z, rot.w));
		PxMaterial* material = engine.gPhysics->createMaterial(mat.staticFriction, mat.dynamicFriction, mat.bounce);

		if (type == DYNAMIC)
			actor = PxCreateDynamic(*engine.gPhysics, transform, PxBoxGeometry(dx / 2, dy / 2, dz / 2), *material, 1.0f);
		else
			actor = PxCreateStatic(*engine.gPhysics, transform, PxBoxGeometry(dx / 2, dy / 2, dz / 2), *material);

		actor->userData = this;
		engine.gScene->addActor(*actor);
	}
}
