#pragma once

#include "Vector3.h"
#include "Quaternion.h"

namespace Cookie
{
	struct Transform
	{
		Transform();
		
		Vector3<> Pos;
		Vector3<> Scale;
		Quaternion Quat;
	};
}
