#pragma once

#include "Vector3.h"
#include "Vector4.h"

namespace Cookie
{
	template<class T = float>
	class Quaternion : public Vector4<T>
	{
	public:
		Quaternion();
		Quaternion(T x, T y, T z, T w);
		static Quaternion<T> FromDirection(T radAngle, Vector3<T> unitDir);
		static Quaternion<T> FromYawPitchRoll(T yaw, T pitch, T roll);
	};

	template<class T>
	Quaternion<T>::Quaternion()
	{
	}

	template<class T>
	Quaternion<T>::Quaternion(T x, T y, T z, T w)
	{
		Vector4<T>::x = x;
		Vector4<T>::y = y;
		Vector4<T>::z = z;
		Vector4<T>::w = w;
	}

	template <class T>
	Quaternion<T> Quaternion<T>::FromDirection(T radAngle, Vector3<T> unitDir)
	{
		float const a = radAngle * 0.5f;
		float const s = sin(a);

		Quaternion<T> q;
		q.w = cos(a);
		q.x = unitDir.x * s;
		q.y = unitDir.y * s;
		q.z = unitDir.z * s;

		return q;
	}

	template<class T>
	Quaternion<T> Quaternion<T>::FromYawPitchRoll(T yaw, T pitch, T roll)
	{
		double cy = cos(yaw * 0.5);
		double sy = sin(yaw * 0.5);
		double cp = cos(pitch * 0.5);
		double sp = sin(pitch * 0.5);
		double cr = cos(roll * 0.5);
		double sr = sin(roll * 0.5);

		Quaternion<T> q;
		q.w = cy * cp * cr + sy * sp * sr;
		q.x = cy * cp * sr - sy * sp * cr;
		q.y = sy * cp * sr + cy * sp * cr;
		q.z = sy * cp * cr - cy * sp * sr;

		return q;
	}
}
