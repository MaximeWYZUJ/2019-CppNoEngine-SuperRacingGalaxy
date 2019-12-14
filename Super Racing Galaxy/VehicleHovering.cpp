#include "VehicleHovering.h"
#include "Vector3.h"
#include "Quaternion.h"
#include "Vehicle.h"
#include "PhysicsEngine.h"
#include <cmath>

namespace Srg
{
	using namespace std;
	using namespace Cookie;

	Vector3<> ComputeRepulsion(Vector3<> raycast, float hitDistance, float maxHitDistance, float planetGravity, Vector3<> curVelocity)
	{
		using namespace Cookie;
		
		Vector3<> force = Vector3<>::Normalize(-raycast);
		float heightInv = 1.0f - hitDistance / maxHitDistance;
		float forceFactor = heightInv * heightInv * heightInv * heightInv * heightInv; // x^5
		float maxPulsion = planetGravity * -20.0f; // 10 times the gravity
		Vector3<> repulsion = force * forceFactor * maxPulsion;
		auto projection = Vector3<>::Projection(curVelocity, repulsion);

		if (projection.first > 0.0f)
		{
			repulsion *= 0.5f;
		}

		return repulsion;
	}

	VehicleHovering::VehicleHovering(PhysicsEngine* physics)
		: physics(physics)
	{
	}

	void VehicleHovering::Update(Vehicle* vehicle, float gravityValue, Vector3<> planetUp)
	{
		Vector3<> vehiclePos = vehicle->root->localTransform.GetPosition();
		Vector3<> vehicleScale = vehicle->root->localTransform.GetScale();
		Quaternion<> vehicleRot = vehicle->root->localTransform.GetRotation();
		
		float rayY = -vehicleScale.y / 2.0f - 0.001f;
		Vector3<> flPoint = vehicleRot * Vector3<>(-vehicleScale.x / 2.0f, rayY, vehicleScale.z / 2.0f) + vehiclePos;
		Vector3<> frPoint = vehicleRot * Vector3<>(vehicleScale.x / 2.0f, rayY, vehicleScale.z / 2.0f) + vehiclePos;
		Vector3<> blPoint = vehicleRot * Vector3<>(-vehicleScale.x / 2.0f, rayY, -vehicleScale.z / 2.0f) + vehiclePos;
		Vector3<> brPoint = vehicleRot * Vector3<>(vehicleScale.x / 2.0f, rayY, -vehicleScale.z / 2.0f) + vehiclePos;

		Vector3<> flRay = vehicleRot * Vector3<>(0.0f, -1.0f, 0.0f);
		Vector3<> frRay = vehicleRot * Vector3<>(0.0f, -1.0f, 0.0f);
		Vector3<> blRay = vehicleRot * Vector3<>(0.0f, -1.0f, 0.0f);
		Vector3<> brRay = vehicleRot * Vector3<>(0.0f, -1.0f, 0.0f);

		auto a = physics->PlanetRaycast(flPoint, flRay, 10.0f);
		auto b = physics->PlanetRaycast(frPoint, frRay, 10.0f);
		auto c = physics->PlanetRaycast(blPoint, blRay, 10.0f);
		auto d = physics->PlanetRaycast(brPoint, brRay, 10.0f);

		Vector3<> curVelocity = vehicle->root->physics->velocity;
		
		if (a.first && b.first && c.first && d.first)
		{
			vehicle->root->physics->addForce(ComputeRepulsion(flRay, a.second, 10.0f, gravityValue, curVelocity));
			vehicle->root->physics->addForce(ComputeRepulsion(frRay, b.second, 10.0f, gravityValue, curVelocity));
			vehicle->root->physics->addForce(ComputeRepulsion(blRay, c.second, 10.0f, gravityValue, curVelocity));
			vehicle->root->physics->addForce(ComputeRepulsion(brRay, d.second, 10.0f, gravityValue, curVelocity));

			Vector3<> forward = vehicleRot * Vector3<>::Forward();
			Vector3<> right = vehicleRot * Vector3<>::Right();
			
			// X+ angle correction
			float leftDiff = a.second - c.second;
			float rightDiff = b.second - d.second;
			float avgHeightDiff = (leftDiff + rightDiff) / 2.0f;
			float radAngle = tan(avgHeightDiff / vehicleScale.z);
			Quaternion<> addRot = Quaternion<>::FromDirection(radAngle, right);
			vehicle->root->localTransform.SetRotation(Quaternion<>::Slerp(vehicleRot, addRot * vehicleRot, 0.5f));
			vehicleRot = vehicle->root->localTransform.GetRotation();
			
			// Z+ angle change
			float frontDiff = a.second - b.second;
			float backDiff = c.second - d.second;
			avgHeightDiff = (frontDiff + backDiff) / 2.0f;
			radAngle = tan(avgHeightDiff / vehicleScale.x);
			addRot = Quaternion<>::FromDirection(radAngle, forward);
			vehicle->root->localTransform.SetRotation(Quaternion<>::Slerp(vehicleRot, addRot * vehicleRot, 0.5f));
		}
		else
		{
			// Align ship with planet up axis
			Vector3<> up = vehicleRot * Vector3<>::Up();
			Quaternion<> rot = Quaternion<>::FromVectorToVector(up, planetUp);
			vehicle->root->localTransform.SetRotation(Quaternion<>::Slerp(vehicleRot, rot * vehicleRot, 0.5f));
		}

		vehicle->root->physics->SetAngularVelocity(Vector3<>::Zero());
	}
}
