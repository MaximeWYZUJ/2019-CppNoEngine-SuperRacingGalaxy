#pragma once
#include "PhysicCollisionCallback.h"
#include "PhysicsComponent.h"

namespace Cookie {

	struct PhysicsTask
	{
		PhysicsComponent* taskOrigin;
		PhysicsComponent* taskDestination;
		PhysicsCollisionCallback* f;

		void job() const {
			(*f)(taskOrigin, taskDestination);
		}
	};

}