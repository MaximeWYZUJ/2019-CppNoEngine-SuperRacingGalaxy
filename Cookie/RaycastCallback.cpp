#include "pch.h"
#include "RaycastCallback.h"
#include "PxShape.h"
#include "PxRigidActor.h"

namespace Cookie
{
	using namespace physx;
	
	PxQueryHitType::Enum RaycastCallback::preFilter(
		PxFilterData const& filterData, PxShape const* shape,
		PxRigidActor const* actor, PxHitFlags& queryFlags)
	{
		auto simFilter = shape->getSimulationFilterData();
		if (simFilter.word0 & (PLANET | LANDING))
		{
			return PxQueryHitType::eBLOCK;
		}

		return PxQueryHitType::eNONE;
	}
	
	PxQueryHitType::Enum RaycastCallback::postFilter(const PxFilterData& filterData, const PxQueryHit& hit)
	{
		auto simFilter = hit.shape->getSimulationFilterData();
		if (simFilter.word0 & (PLANET | LANDING))
		{
			return PxQueryHitType::eBLOCK;
		}

		return PxQueryHitType::eNONE;
	}
}