#pragma once
//#include "PhysicComponent.h"

namespace Cookie {

	class PhysicComponent;

	class PhysicCollisionCallback
	{
	public:
		PhysicCollisionCallback() = default;
		~PhysicCollisionCallback() = default;
		virtual void operator()(PhysicComponent* otherComponent) {};
	};

}
