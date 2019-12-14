#pragma once
#include <iostream>

namespace Cookie {

	class PhysicsComponent;

	class PhysicsCollisionCallback
	{
	public:
		PhysicsCollisionCallback() = default;
		~PhysicsCollisionCallback() = default;
		virtual void operator()(PhysicsComponent* selfComponent, PhysicsComponent* otherComponent) { std::cout << "no specific callback" << std::endl; };
	};
}
