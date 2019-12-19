#pragma once
#include "Component.h"

namespace Cookie
{
	class Billboard : public Component
	{
	public:
		void Update(Quaternion<> cameraRot);
	};
}
