#pragma once

namespace Cookie
{
	class PhysicMaterial
	{
	public:
		float bounce;
		float staticFriction;
		float dynamicFriction;

		PhysicMaterial(float b, float s, float d)
			:bounce(b), staticFriction(s), dynamicFriction(d)
		{

		}

		PhysicMaterial() 
			:bounce(0.5f), staticFriction(0.5f), dynamicFriction(0.5f) 
		{

		}
	};
}

