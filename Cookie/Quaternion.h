#pragma once

#include "Vector3.h"
#include "Vector4.h"

namespace Cookie
{
	template<class T = float>
	class Quaternion : public Vector4<T>
	{
	public:
		static constexpr Quaternion<T> Identity();
		static Quaternion<T> FromDirection(T radAngle, Vector3<T> unitDir);
		static Quaternion<T> FromVectorToVector(Vector3<T> const& unitV1, Vector3<T> const& unitV2);
		static Quaternion<T> FromYawPitchRoll(T yaw, T pitch, T roll);
		static Quaternion<T> Slerp(Quaternion<T> const& lhs, Quaternion<T> const& rhs, float t);
		static T DotProduct(Quaternion<T> const& lhs, Quaternion<T> const& rhs);

		Quaternion();
		Quaternion(T x, T y, T z, T w);
	};

	template <class T>
	constexpr Quaternion<T> Quaternion<T>::Identity()
	{
		return Quaternion<T>(0.0f, 0.0f, 0.0f, 1.0f);
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
	Quaternion<T> Quaternion<T>::FromVectorToVector(Vector3<T> const& unitV1, Vector3<T> const& unitV2)
	{
		Quaternion q;
		T d = Vector3<>::DotProduct(unitV1, unitV2);

		if (d >= 1.0f)
		{
			return Quaternion<T>::Identity();
		}

		if (d < 1e-6f - 1.0f)
		{
			Vector3<> axis = Vector3<T>::CrossProduct(Vector3<T>(1.0f, 0.0f, 0.0f), unitV1);
			if (axis.IsApproximatelyZero()) // pick another if colinear
				axis = Vector3<>::CrossProduct(Vector3<T>(0.0f, 1.0f, 0.0f), unitV2);
			axis.Normalize();
			q = Quaternion<>::FromDirection(Math::Pi, axis);
		}
		else
		{
			T s = sqrt((1 + d) * 2);
			T inv = 1 / s;

			Vector3 c = Vector3<>::CrossProduct(unitV1, unitV2);

			q.x = c.x * inv;
			q.y = c.y * inv;
			q.z = c.z * inv;
			q.w = s * 0.5f;
			q.Normalize4d();
		}

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

	template<class T>
	T Quaternion<T>::DotProduct(Quaternion<T> const& lhs, Quaternion<T> const& rhs)
	{
		return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
	}

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

	template<class T>
	Quaternion<T> operator*(Quaternion<T> const& lhs, Quaternion<T> const& rhs)
	{
		return Quaternion(
			lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y,
			lhs.w * rhs.y - lhs.x * rhs.z + lhs.y * rhs.w + lhs.z * rhs.x,
			lhs.w * rhs.z + lhs.x * rhs.y - lhs.y * rhs.x + lhs.z * rhs.w,
			lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z);
	}

	template<class T>
	Quaternion<T> operator*(float lhs, Quaternion<T> const& rhs)
	{
		return Quaternion<T>(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z, lhs * rhs.w);
	}

	template<class T>
	Vector4<T> operator*(Quaternion<T> const& lhs, Vector4<T> const& rhs)
	{
		return (lhs * Quaternion<T>(rhs.x, rhs.y, rhs.z, rhs.w)) * Quaternion<T>(-lhs.x, -lhs.y, -lhs.z, lhs.w);
	}

	template<class T>
	Vector3<T> operator*(Quaternion<T> const& lhs, Vector3<T> const& rhs)
	{
		return (lhs * Quaternion<T>(rhs.x, rhs.y, rhs.z, 1.0f)) * Quaternion<T>(-lhs.x, -lhs.y, -lhs.z, lhs.w);
	}

	template<class T>
	Quaternion<T> operator-(Quaternion<T> const& lhs)
	{
		return Quaternion<T>(-lhs.x, -lhs.y, -lhs.z, -lhs.w);
	}

	template<class T>
	Quaternion<T> operator-(Quaternion<T> const& lhs, Quaternion<T> const& rhs)
	{
		return Quaternion<T>(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
	}

	template<class T>
	Quaternion<T> operator+(Quaternion<T> const& lhs, Quaternion<T> const& rhs)
	{
		return Quaternion<T>(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
	}

	template <class T>
	Quaternion<T> Quaternion<T>::Slerp(Quaternion<T> const& lhs, Quaternion<T> const& rhs, float t)
	{
		// From Wikipedia

		Quaternion<T> v0 = lhs;
		Quaternion<T> v1 = rhs;

		// Only unit quaternions are valid rotations.
		// Normalize to avoid undefined behavior.
		v0.Normalize4d();
		v1.Normalize4d();

		// Compute the cosine of the angle between the two vectors.
		double dot = Quaternion<>::DotProduct(v0, v1);

		// If the dot product is negative, slerp won't take
		// the shorter path. Note that v1 and -v1 are equivalent when
		// the negation is applied to all four components. Fix by 
		// reversing one quaternion.
		if (dot < 0.0f) {
			v1 = -v1;
			dot = -dot;
		}

		const double DOT_THRESHOLD = 0.9995;
		if (dot > DOT_THRESHOLD) {
			// If the inputs are too close for comfort, linearly interpolate
			// and normalize the result.

			Quaternion<> result = v0 + t * (v1 - v0);
			result.Normalize4d();
			return result;
		}

		// Since dot is in range [0, DOT_THRESHOLD], acos is safe
		double theta_0 = acos(dot);        // theta_0 = angle between input vectors
		double theta = theta_0 * t;          // theta = angle between v0 and result
		double sin_theta = sin(theta);     // compute this value only once
		double sin_theta_0 = sin(theta_0); // compute this value only once

		float s0 = cos(theta) - dot * sin_theta / sin_theta_0;  // == sin(theta_0 - theta) / sin(theta_0)
		float s1 = sin_theta / sin_theta_0;

		return (s0 * v0) + (s1 * v1);
	}
}
