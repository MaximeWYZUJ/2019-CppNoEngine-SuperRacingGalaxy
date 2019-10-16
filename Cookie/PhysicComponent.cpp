#include "pch.h"

#include <algorithm>

#include "PhysicComponent.h"

namespace Cookie {
	PhysicComponent::PhysicComponent(Vector3<float> pos, Quaternion rot, PhysicMaterial mat, bodyType _type)
		: position(pos), rotation(rot), material(mat), type(_type), actor(nullptr)
	{

	}

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
}