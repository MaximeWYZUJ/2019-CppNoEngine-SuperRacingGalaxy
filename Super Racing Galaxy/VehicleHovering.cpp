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
		float maxPulsion = planetGravity * -1.4f; // 1 times the gravity
		Vector3<> repulsion = force * forceFactor * maxPulsion;
		auto projection = Vector3<>::Projection(curVelocity, repulsion);

		if (projection.first > 0.0f)
		{
			repulsion *= 0.7f;
		}

		return repulsion;
	}

	VehicleHovering::VehicleHovering(PhysicsEngine* physics)
		: physics(physics)
	{
	}

	void VehicleHovering::Update(Vehicle* vehicle, float gravityValue, Vector3<> planetUp, bool isPlayerMoving)
	{
		static float rayDistance = 10.0f;
		static float expectedHeight = 2.0f;
		static float cut = 5.0f;
		
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
		
		Vector3<> curVelocity = vehicle->root->physics->velocity;

		auto a = physics->PlanetRaycast(flPoint, flRay, rayDistance);
		auto b = physics->PlanetRaycast(frPoint, frRay, rayDistance);
		auto c = physics->PlanetRaycast(blPoint, blRay, rayDistance);
		auto d = physics->PlanetRaycast(brPoint, brRay, rayDistance);
		
		float maxHitDistance = max(max(max(a.second, b.second), c.second), d.second);

		if (a.first && b.first && c.first && d.first)
		{
			vehicle->root->physics->addForce(ComputeRepulsion(flRay, clamp(a.second, 0.0f, cut), rayDistance - cut, gravityValue, curVelocity));
			vehicle->root->physics->addForce(ComputeRepulsion(frRay, clamp(b.second, 0.0f, cut), rayDistance - cut, gravityValue, curVelocity));
			vehicle->root->physics->addForce(ComputeRepulsion(blRay, clamp(c.second, 0.0f, cut), rayDistance - cut, gravityValue, curVelocity));
			vehicle->root->physics->addForce(ComputeRepulsion(brRay, clamp(d.second, 0.0f, cut), rayDistance - cut, gravityValue, curVelocity));

			float avgDistance = (a.second + b.second + c.second + d.second) / 4.0f;
			float slerpTime = 1.0f;

			if (avgDistance > expectedHeight)
			{
				slerpTime = 1.0f - (avgDistance - expectedHeight) / (rayDistance - expectedHeight);
			}

			Vector3<> forward = vehicleRot * Vector3<>::Forward();
			Vector3<> right = vehicleRot * Vector3<>::Right();

			// X+ angle correction
			float leftDiff = a.second - c.second;
			float rightDiff = b.second - d.second;
			float avgHeightDiff = (leftDiff + rightDiff) / 2.0f;
			float radAngle = tan(avgHeightDiff / vehicleScale.z);
			Quaternion<> addRot = Quaternion<>::FromDirection(radAngle, right);
			vehicle->root->localTransform.SetRotation(Quaternion<>::Slerp(vehicleRot, addRot * vehicleRot, slerpTime));
			vehicleRot = vehicle->root->localTransform.GetRotation();

			// Z+ angle change
			float frontDiff = a.second - b.second;
			float backDiff = c.second - d.second;
			avgHeightDiff = (frontDiff + backDiff) / 2.0f;
			radAngle = tan(avgHeightDiff / vehicleScale.x);
			addRot = Quaternion<>::FromDirection(radAngle, forward);
			vehicle->root->localTransform.SetRotation(Quaternion<>::Slerp(vehicleRot, addRot * vehicleRot, slerpTime));
		}
		else if (a.first || b.first || c.first || d.first)
		{
			vehicle->root->physics->addForce(ComputeRepulsion(flRay, clamp(maxHitDistance, 0.0f, cut), rayDistance - cut, gravityValue, curVelocity));
			vehicle->root->physics->addForce(ComputeRepulsion(frRay, clamp(maxHitDistance, 0.0f, cut), rayDistance - cut, gravityValue, curVelocity));
			vehicle->root->physics->addForce(ComputeRepulsion(blRay, clamp(maxHitDistance, 0.0f, cut), rayDistance - cut, gravityValue, curVelocity));
			vehicle->root->physics->addForce(ComputeRepulsion(brRay, clamp(maxHitDistance, 0.0f, cut), rayDistance - cut, gravityValue, curVelocity));
		}
		else
		{
			// Align ship with planet up axis
			float slerpTime = 0.1f;
			Vector3<> up = vehicleRot * Vector3<>::Up();
			Quaternion<> rot = Quaternion<>::FromVectorToVector(up, planetUp);
			vehicle->root->localTransform.SetRotation(Quaternion<>::Slerp(vehicleRot, rot * vehicleRot, slerpTime));
		}
		
		if (!isPlayerMoving && maxHitDistance > 0.0f && maxHitDistance < 4.0f)
		{
			// Dampening
			vehicle->root->physics->addForce(curVelocity * gravityValue * 0.01f);
		}

		vehicle->root->physics->SetAngularVelocity(Vector3<>::Zero());
	}
}
