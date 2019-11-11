#pragma once

namespace Cookie
{
	template<class T = float>
	class Vector2
	{
	public:
		Vector2() = default;
		Vector2(T x, T y);

		T x;
		T y;
	};

	template<class T>
	Vector2<T>::Vector2(T x, T y)
		: x(x), y(y)
	{
	}

	template<class T>
	Vector2<T> operator-(Vector2<T> const& lhs, Vector2<T> const& rhs)
	{
		return Vector2<T>(lhs.x - rhs.x, lhs.y - rhs.y);
	}
}