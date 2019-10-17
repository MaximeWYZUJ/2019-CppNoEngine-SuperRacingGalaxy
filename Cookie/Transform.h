#pragma once

#include "Vector3.h"
#include "Quaternion.h"

namespace Cookie
{
	template<class T = float>
	class Transform
	{
	public:
		Transform();

		Vector3<T> pos;
		Vector3<T> scale;
		Quaternion<T> rotation;
	};

	template<class T>
	Transform<T>::Transform()
		: pos(static_cast<T>(0), static_cast<T>(0), static_cast<T>(0)),
		  scale(static_cast<T>(1), static_cast<T>(1), static_cast<T>(1)),
		  rotation(static_cast<T>(1), static_cast<T>(0), static_cast<T>(0), static_cast<T>(0))
	{
	}
}