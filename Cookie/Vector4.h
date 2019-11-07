#pragma once

namespace Cookie
{
	template <class T = float>
	class Vector4
	{
	public:
		T x;
		T y;
		T z;
		T w;

		Vector4<T>() : x(0), y(0), z(0), w(0) {}
		Vector4<T>(T X, T Y, T Z, T W) : x(X), y(Y), z(Z), w(W) {}
		Vector4<T>(const Vector4<T>& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}
	};
}