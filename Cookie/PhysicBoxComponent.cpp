#include "pch.h"
#include "PhysicBoxComponent.h"
#include "PhysicEngine.h"

using namespace physx;

namespace Cookie {
	PhysicBoxComponent::PhysicBoxComponent(Vector3<float> pos, Quaternion rot, PhysicMaterial mat, bodyType type, float _dx, float _dy, float _dz) 
					  : PhysicComponent(pos, rot, mat, type), dx(_dx), dy(_dy), dz(_dz)
	{
		auto& engine= PhysicEngine::getInstance();
		PxTransform transform(PxVec3(-pos.X, pos.Y, pos.Z));
		PxMaterial* material = engine.gPhysics->createMaterial(mat.staticFriction, mat.dynamicFriction, mat.bounce);

		if (type == DYNAMIC)
			actor = PxCreateDynamic(*engine.gPhysics, transform, PxBoxGeometry(dx / 2, dy / 2, dz / 2), *material, 1.0f);
		else
			actor = PxCreateStatic(*engine.gPhysics, transform, PxBoxGeometry(dx / 2, dy / 2, dz / 2), *material);
		
	}

	void PhysicBoxComponent::addForce()
	{

	}

	void PhysicBoxComponent::onCollisionCallBack()
	{

	}

	void PhysicBoxComponent::onTriggerCallBack()
	{

	}
}