#pragma once

namespace Cookie {

	class PhysicsComponent;

	class PhysicsCollisionCallback
	{
	public:
		PhysicsCollisionCallback() = default;
		~PhysicsCollisionCallback() = default;
		virtual void operator()(PhysicsComponent* otherComponent) {};
	};

}
