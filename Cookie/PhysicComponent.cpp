#include "pch.h"

#include <algorithm>

#include "PhysicComponent.h"

using namespace physx;

namespace Cookie {

	void PhysicComponent::addFilterGroup(FilterGroup f) {
		if (std::find(selfGroup.begin(), selfGroup.end(), f) == selfGroup.end())
			selfGroup.push_back(f);
	}

	void PhysicComponent::removeFilterGroup(FilterGroup f) {
		if (auto it = std::find(selfGroup.begin(), selfGroup.end(), f); it != selfGroup.end())
			selfGroup.erase(it);
	}

	void PhysicComponent::addFilterMask(FilterGroup f) {
		if (std::find(mask.begin(), mask.end(), f) == mask.end())
			mask.push_back(f);
	}

	void PhysicComponent::removeFilterMask(FilterGroup f) {
		if (auto it = std::find(mask.begin(), mask.end(), f); it != mask.end())
			mask.erase(it);
	}
	void PhysicComponent::updateFilters()
	{
		int nbShapes = actor->getNbShapes();
		if (nbShapes == 0) {
			throw NoShapeException{};
		}

		PxShape** shapes{};
		actor->getShapes(shapes, sizeof(PxShape) * nbShapes);


		PxFilterData filterData;
		std::for_each(selfGroup.begin(), selfGroup.end(), [&filterData](FilterGroup f) {
			filterData.word0 |= f;
		});
		std::for_each(mask.begin(), mask.end(), [&filterData](FilterGroup f) {
			filterData.word1 |= f;
		});

		for (int i = 0; i < nbShapes; i++) {
			shapes[i]->setSimulationFilterData(filterData);
		}
	}
}