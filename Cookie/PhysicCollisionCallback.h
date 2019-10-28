#pragma once
//#include "PhysicComponent.h"
#include <fstream>

namespace Cookie {

	class PhysicComponent;

	class PhysicCollisionCallback
	{
	public:
		PhysicCollisionCallback() = default;
		~PhysicCollisionCallback() = default;
		virtual void operator()(PhysicComponent* otherComponent) {
			std::fstream file("output.txt", std::ofstream::app);
			file << "truc generique" << std::endl;
		};
	};

}
