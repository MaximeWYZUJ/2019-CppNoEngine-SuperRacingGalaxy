#pragma once

namespace Cookie
{
	template <class T = float>
	class Vector4
	{
	public:
		static Vector4<T> CrossProduct(Vector4<T> const& lhs, Vector4<T> const& rhs);
		static Vector4<T> Normalize(Vector4<T> const& v);
		
		Vector4<T>() : x(0), y(0), z(0), w(0) {}
		Vector4<T>(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
		Vector4<T>(Vector4<T> const& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}

		T x;
		T y;
		T z;
		T w;
	};

	template<class T>
	Vector4<T> Vector4<T>::CrossProduct(Vector4<T> const& lhs, Vector4<T> const& rhs)
	{
		return Vector4<T>(
			lhs.y * rhs.z - lhs.z * rhs.y,
			lhs.z * rhs.x - lhs.x * rhs.z,
			lhs.x * rhs.y - lhs.y * rhs.x,
			1.0f);
	}

	template<class T>
	Vector4<T> Vector4<T>::Normalize(Vector4<T> const& v)
	{
		float len = static_cast<float>(sqrt(v.x * v.x + v.y * v.y + v.z * v.z));
		return Vector4<T>(v.x / len, v.y / len, v.z / len, v.w);
	}

	template<class T>
	Vector4<T> operator*(Vector4<T> const& v, T scalar)
	{
		return Vector4<T>(v.x * scalar, v.y * scalar, v.z * scalar, v.w);
	}

	template<class T>
	Vector4<T> operator-(Vector4<T> const& v)
	{
		return Vector4<T>(-v.x, -v.y, -v.z, v.w);
	}
}