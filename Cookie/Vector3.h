#pragma once

#include <iostream>
#include "CookieMath.h"

namespace Cookie
{
	template<typename T = float>
	class Vector4;

	template<typename T = float>
	class Vector3
	{
	public:
		static constexpr Vector3<T> Zero();
		static constexpr Vector3<T> Forward();
		static constexpr Vector3<T> Up();
		static constexpr Vector3<T> Right();
		static constexpr Vector3<T> Unit();
		static Vector3<T> Normalize(Vector3<T> const& v);
		static T DotProduct(Vector3<T> const& lhs, Vector3<T> const& rhs);
		static Vector3<T> CrossProduct(Vector3<T> const& lhs, Vector3<T> const& rhs);
		static T Distance(Vector3<T> const& lhs, Vector3<T> const& rhs);
		static Vector3<T> Lerp(Vector3<T> const& lhs, Vector3<T> const& rhs, T time);
		static std::pair<float, Vector3<>> Projection(Vector3<> const& a, Vector3<> const& b);
		
		Vector3<T>() : x(0), y(0), z(0) {}
		Vector3<T>(T X, T Y, T Z) : x(X), y(Y), z(Z) {}
		Vector3<T>(const Vector3<T>& v) : x(v.x), y(v.y), z(v.z) {}
		Vector3<T>(const Vector4<T>& v);

		float distance(const Vector3<T>& v) const;
		float squareDistance(const Vector3<T>& v) const;

		float operator%(const Vector3<T>& v) const;				// produit scalaire
		Vector3<T> operator^(const Vector3<T>& v) const;		// produit vectoriel
		void Normalize();

		Vector3<T> operator+ (const Vector3<T>& v) const;
		Vector3<T> operator+ (T d) const;
		Vector3<T> operator- (const Vector3<T>& v) const;
		Vector3<T> operator- (T d) const;
		Vector3<T> operator- () const;

		Vector3<T> operator*  (const Vector3<T>& v) const;
		Vector3<T> operator*  (T d) const;
		Vector3<T> operator/  (const Vector3<T>& v) const;
		Vector3<T> operator/  (T d) const;

		bool operator== (const Vector3<T>& v) const;
		bool operator!= (const Vector3<T>& v) const;

		Vector3<T>& operator=  (const Vector3<T>& v);
		Vector3<T>& operator+= (const Vector3<T>& v);
		Vector3<T>& operator-= (const Vector3<T>& v);
		Vector3<T>& operator+= (T d);
		Vector3<T>& operator-= (T d);
		Vector3<T>& operator*= (T d);
		Vector3<T>& operator/= (T d);

		friend Vector3<T> operator+ (T d, const Vector3<T>& v)
		{
			return Vector3<T>(v.x + d, v.y + d, v.z + d);
		}

		friend Vector3<T> operator* (T d, const Vector3<T>& v)
		{
			return Vector3<T>(v.x * d, v.y * d, v.z * d);
		}

		friend std::ostream& operator<< (std::ostream& os, const Vector3<T>& v)
		{
			return os << v.x << ", " << v.y << ", " << v.z;
		}

		[[nodiscard]]
		T Length() const;

		[[nodiscard]]
		bool IsApproximatelyZero() const;

		T x, y, z;
	};

	template<class T>
	constexpr Vector3<T> Vector3<T>::Zero()
	{
		return Vector3<T>(0.0f, 0.0f, 0.0f);
	}

	template<class T>
	constexpr Vector3<T> Vector3<T>::Forward()
	{
		return Vector3<T>(0.0f, 0.0f, 1.0f);
	}

	template<class T>
	constexpr Vector3<T> Vector3<T>::Up()
	{
		return Vector3<T>(0.0f, 1.0f, 0.0f);
	}

	template<class T>
	constexpr Vector3<T> Vector3<T>::Right()
	{
		return Vector3<T>(1.0f, 0.0f, 0.0f);
	}

	template<class T>
	constexpr Vector3<T> Vector3<T>::Unit()
	{
		return Vector3<T>(1.0f, 1.0f, 1.0f);
	}

	template<class T>
	Vector3<T> Vector3<T>::Normalize(Vector3<T> const& v)
	{
		float len = static_cast<float>(sqrt(v.x * v.x + v.y * v.y + v.z * v.z));
		return Vector3<T>(v.x / len, v.y / len, v.z / len);
	}

	template<class T>
	T Vector3<T>::DotProduct(Vector3<T> const& lhs, Vector3<T> const& rhs)
	{
		return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
	}

	template<typename T>
	Vector3<T> Vector3<T>::CrossProduct(Vector3<T> const& lhs, Vector3<T> const& rhs)
	{
		return Vector3<T>(
			lhs.y * rhs.z - lhs.z * rhs.y,
			lhs.z * rhs.x - lhs.x * rhs.z,
			lhs.x * rhs.y - lhs.y * rhs.x);
	}

	template<typename T>
	T Vector3<T>::Distance(Vector3<T> const& lhs, Vector3<T> const& rhs)
	{
		return (lhs - rhs).Length();
	}

	template<typename T>
	Vector3<T> Vector3<T>::Lerp(Vector3<T> const& lhs, Vector3<T> const& rhs, T ratio)
	{
		return Vector3<T>(Math::Lerp(lhs.x, rhs.x, ratio), Math::Lerp(lhs.y, rhs.y, ratio), Math::Lerp(lhs.z, rhs.z, ratio));
	}

	template<class T>
	std::pair<float, Vector3<>> Vector3<T>::Projection(Vector3<> const& a, Vector3<> const& b)
	{
		// Projection of a onto b
		auto bLength = b.Length();
		auto dot = Vector3<>::DotProduct(b, a);
		return { dot, dot / (bLength * bLength) * b };
	}

	template<typename T>
	Vector3<T>::Vector3(const Vector4<T>& v)
		: x(v.x), y(v.y), z(v.z)
	{
	}

	template<typename T>
	float Vector3<T>::operator% (const Vector3<T>& v) const
	{
		return x * v.x + y * v.y + z * v.z;
	}

	template<typename T>
	Vector3<T> Vector3<T>::operator^ (const Vector3<T>& v) const
	{
		return Vector3<T>(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}


	template<typename T>
	void Vector3<T>::Normalize()
	{
		float len = static_cast<float>(sqrt(x * x + y * y + z * z));

		if (len == 0)
		{
			x = 0;
			y = 0;
			z = 0;
		}
		else
		{
			x /= len;
			y /= len;
			z /= len;
		}
	}


	template<typename T>
	Vector3<T> Vector3<T>::operator+ (const Vector3<T>& v) const
	{
		return Vector3<T>(x + v.x, y + v.y, z + v.z);
	}


	template<typename T>
	Vector3<T> Vector3<T>::operator+ (T d) const
	{
		return Vector3<T>(x + d, y + d, z + d);
	}


	template<typename T>
	Vector3<T> operator+ (const Vector3<T>& v1, const Vector3<T>& v2)
	{
		return Vector3<T>(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
	}


	template<typename T>
	Vector3<T> Vector3<T>::operator- (const Vector3<T>& v) const
	{
		return Vector3<T>(x - v.x, y - v.y, z - v.z);
	}


	template<typename T>
	Vector3<T> Vector3<T>::operator- (T d) const
	{
		return Vector3<T>(x - d, y - d, z - d);
	}


	template<typename T>
	Vector3<T> operator- (const Vector3<T>& v1, const Vector3<T>& v2)
	{
		return Vector3<T>(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
	}


	template<typename T>
	Vector3<T> Vector3<T>::operator- () const
	{
		return Vector3<T>(-x, -y, -z);
	}


	template<typename T>
	Vector3<T> Vector3<T>::operator* (const Vector3<T>& v) const
	{
		return Vector3<T>(x * v.x, y * v.y, z * v.z);
	}


	template<typename T>
	Vector3<T> operator* (const Vector3<T>& v1, const Vector3<T>& v2)
	{
		return Vector3<T>(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
	}


	template<typename T>
	Vector3<T> Vector3<T>::operator* (T d) const
	{
		return Vector3<T>(x * d, y * d, z * d);
	}


	template<typename T>
	Vector3<T> Vector3<T>::operator/ (T d) const
	{
		return Vector3<T>(x / d, y / d, z / d);
	}


	template<typename T>
	Vector3<T> Vector3<T>::operator/ (const Vector3<T>& v) const
	{
		return Vector3<T>(x / v.x, y / v.y, z / v.z);
	}


	template<typename T>
	Vector3<T> operator/ (const Vector3<T>& v1, const Vector3<T>& v2)
	{
		return Vector3<T>(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z);
	}


	template<typename T>
	bool Vector3<T>::operator== (const Vector3<T>& v) const
	{
		return((x == v.x) && (y == v.y) && (z == v.z));
	}

	template<typename T>
	bool Vector3<T>::operator!= (const Vector3<T>& v) const
	{
		return((x != v.x) || (y != v.y) || (z != v.z));
	}

	template<typename T>
	Vector3<T>& Vector3<T>::operator= (const Vector3<T>& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}


	template<typename T>
	Vector3<T>& Vector3<T>::operator+= (const Vector3<T>& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	template<typename T>
	Vector3<T>& Vector3<T>::operator-= (const Vector3<T>& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}


	template<typename T>
	Vector3<T>& Vector3<T>::operator+= (T d)
	{
		x += d;
		y += d;
		z += d;
		return *this;
	}


	template<typename T>
	Vector3<T>& Vector3<T>::operator-= (T d)
	{
		x -= d;
		y -= d;
		z -= d;
		return *this;
	}


	template<typename T>
	Vector3<T>& Vector3<T>::operator*= (T d)
	{
		x *= d;
		y *= d;
		z *= d;
		return *this;
	}


	template<typename T>
	Vector3<T>& Vector3<T>::operator/= (T d)
	{
		x /= d;
		y /= d;
		z /= d;
		return *this;
	}


	template<typename T>
	T Vector3<T>::Length() const
	{
		return static_cast<T>(sqrt(x * x + y * y + z * z));
	}

	template<typename T>
	bool Vector3<T>::IsApproximatelyZero() const
	{
		return Math::Approximately(Length(), 0.0f);
	}


	template<typename T>
	float Vector3<T>::distance(const Vector3<T>& v) const
	{
		return static_cast<float>(sqrt((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y) + (z - v.z) * (z - v.z)));
	}

	template<typename T>
	float Vector3<T>::squareDistance(const Vector3<T>& v) const
	{
		return static_cast<float>((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y) + (z - v.z) * (z - v.z));
	}
}
