#pragma once

#include <iostream>
#include "Vector4.h"

namespace Cookie
{
	template<typename T = float>
	class Vector3
	{
	public:
		Vector3<T>() : x(0), y(0), z(0) {}
		Vector3<T>(T X, T Y, T Z) : x(X), y(Y), z(Z) {}
		Vector3<T>(const Vector3<T>& v) : x(v.x), y(v.y), z(v.z) {}
		Vector3<T>(const Vector4<T>& v) : x(v.x), y(v.y), z(v.z) {}

		float distance(const Vector3<T>& v) const;
		float squareDistance(const Vector3<T>& v) const;

		float operator%(const Vector3<T>& v) const;				// produit scalaire
		Vector3<T> operator^(const Vector3<T>& v) const;		// produit vectoriel
		void normalize();

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

		float length() const;

		T x, y, z;
	};

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
	void Vector3<T>::normalize()
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
	float Vector3<T>::length() const
	{
		return static_cast<float>(sqrt(x * x + y * y + z * z));
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
