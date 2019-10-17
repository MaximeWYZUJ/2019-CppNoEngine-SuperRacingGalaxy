#pragma once

#include "Vector4.h"

namespace Cookie
{
	template<class T = float>
	class Quaternion : public Vector4<T>
	{
	public:
		Quaternion(T x, T y, T z, T w);
	};

	template<class T>
	Quaternion<T>::Quaternion(T x, T y, T z, T w)
	{
		Vector4<T>::x = x;
		Vector4<T>::y = y;
		Vector4<T>::z = z;
		Vector4<T>::w = w;
	}
}
