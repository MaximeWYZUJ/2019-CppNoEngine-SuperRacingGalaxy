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
		Vector4<T>(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
		Vector4<T>(Vector4<T> const& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}
	};
}