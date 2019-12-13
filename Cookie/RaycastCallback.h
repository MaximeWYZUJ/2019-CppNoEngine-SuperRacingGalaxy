#pragma once
#include <PxQueryFiltering.h>

namespace Cookie
{
	class RaycastCallback : public physx::PxQueryFilterCallback
	{
	public:
		physx::PxQueryHitType::Enum preFilter(
			physx::PxFilterData const& filterData,
			physx::PxShape const* shape,
			physx::PxRigidActor const* actor,
			physx::PxHitFlags& queryFlags) override;

		physx::PxQueryHitType::Enum postFilter(
			physx::PxFilterData const& filterData,
			physx::PxQueryHit const& hit) override;

		virtual ~RaycastCallback() = default;
	};
}