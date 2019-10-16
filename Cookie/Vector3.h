#pragma once

#include <iostream>
#include "Vector4.h"

namespace Cookie
{
	template<typename T = float>
	class Vector3
	{
	public:
		Vector3<T>() : X(0), Y(0), Z(0) {}
		Vector3<T>(T X, T Y, T Z) : X(X), Y(Y), Z(Z) {}
		Vector3<T>(const Vector3<T>& v) : X(v.X), Y(v.Y), Z(v.Z) {}
		Vector3<T>(const Vector4<T>& v) : X(v.X), Y(v.Y), Z(v.Z) {}

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
			return Vector3<T>(v.X + d, v.Y + d, v.Z + d);
		}

		friend Vector3<T> operator* (T d, const Vector3<T>& v)
		{
			return Vector3<T>(v.X * d, v.Y * d, v.Z * d);
		}

		friend std::ostream& operator<< (std::ostream& os, const Vector3<T>& v)
		{
			return os << v.X << ", " << v.Y << ", " << v.Z;
		}

		float length() const;


		T X, Y, Z;
	};

	template<typename T>
	float Vector3<T>::operator% (const Vector3<T>& v) const
	{
		return X * v.X + Y * v.Y + Z * v.Z;
	}

	template<typename T>
	Vector3<T> Vector3<T>::operator^ (const Vector3<T>& v) const
	{
		return Vector3<T>(Y * v.Z - Z * v.Y, Z * v.X - X * v.Z, X * v.Y - Y * v.X);
	}


	template<typename T>
	void Vector3<T>::normalize()
	{
		float len = static_cast<float>(sqrt(X * X + Y * Y + Z * Z));

		if (len == 0)
		{
			X = 0;
			Y = 0;
			Z = 0;
		}
		else
		{
			X /= len;
			Y /= len;
			Z /= len;
		}
	}


	template<typename T>
	Vector3<T> Vector3<T>::operator+ (const Vector3<T>& v) const
	{
		return Vector3<T>(X + v.X, Y + v.Y, Z + v.Z);
	}


	template<typename T>
	Vector3<T> Vector3<T>::operator+ (T d) const
	{
		return Vector3<T>(X + d, Y + d, Z + d);
	}


	template<typename T>
	Vector3<T> operator+ (const Vector3<T>& v1, const Vector3<T>& v2)
	{
		return Vector3<T>(v1.X + v2.X, v1.Y + v2.Y, v1.Z + v2.Z);
	}


	template<typename T>
	Vector3<T> Vector3<T>::operator- (const Vector3<T>& v) const
	{
		return Vector3<T>(X - v.X, Y - v.Y, Z - v.Z);
	}


	template<typename T>
	Vector3<T> Vector3<T>::operator- (T d) const
	{
		return Vector3<T>(X - d, Y - d, Z - d);
	}


	template<typename T>
	Vector3<T> operator- (const Vector3<T>& v1, const Vector3<T>& v2)
	{
		return Vector3<T>(v1.X - v2.X, v1.Y - v2.Y, v1.Z - v2.Z);
	}


	template<typename T>
	Vector3<T> Vector3<T>::operator- () const
	{
		return Vector3<T>(-X, -Y, -Z);
	}


	template<typename T>
	Vector3<T> Vector3<T>::operator* (const Vector3<T>& v) const
	{
		return Vector3<T>(X * v.X, Y * v.Y, Z * v.Z);
	}


	template<typename T>
	Vector3<T> operator* (const Vector3<T>& v1, const Vector3<T>& v2)
	{
		return Vector3<T>(v1.X * v2.X, v1.Y * v2.Y, v1.Z * v2.Z);
	}


	template<typename T>
	Vector3<T> Vector3<T>::operator* (T d) const
	{
		return Vector3<T>(X * d, Y * d, Z * d);
	}


	template<typename T>
	Vector3<T> Vector3<T>::operator/ (T d) const
	{
		return Vector3<T>(X / d, Y / d, Z / d);
	}


	template<typename T>
	Vector3<T> Vector3<T>::operator/ (const Vector3<T>& v) const
	{
		return Vector3<T>(X / v.X, Y / v.Y, Z / v.Z);
	}


	template<typename T>
	Vector3<T> operator/ (const Vector3<T>& v1, const Vector3<T>& v2)
	{
		return Vector3<T>(v1.X / v2.X, v1.Y / v2.Y, v1.Z / v2.Z);
	}


	template<typename T>
	bool Vector3<T>::operator== (const Vector3<T>& v) const
	{
		return((X == v.X) && (Y == v.Y) && (Z == v.Z));
	}

	template<typename T>
	bool Vector3<T>::operator!= (const Vector3<T>& v) const
	{
		return((X != v.X) || (Y != v.Y) || (Z != v.Z));
	}

	template<typename T>
	Vector3<T>& Vector3<T>::operator= (const Vector3<T>& v)
	{
		X = v.X;
		Y = v.Y;
		Z = v.Z;
		return *this;
	}


	template<typename T>
	Vector3<T>& Vector3<T>::operator+= (const Vector3<T>& v)
	{
		X += v.X;
		Y += v.Y;
		Z += v.Z;
		return *this;
	}

	template<typename T>
	Vector3<T>& Vector3<T>::operator-= (const Vector3<T>& v)
	{
		X -= v.X;
		Y -= v.Y;
		Z -= v.Z;
		return *this;
	}


	template<typename T>
	Vector3<T>& Vector3<T>::operator+= (T d)
	{
		X += d;
		Y += d;
		Z += d;
		return *this;
	}


	template<typename T>
	Vector3<T>& Vector3<T>::operator-= (T d)
	{
		X -= d;
		Y -= d;
		Z -= d;
		return *this;
	}


	template<typename T>
	Vector3<T>& Vector3<T>::operator*= (T d)
	{
		X *= d;
		Y *= d;
		Z *= d;
		return *this;
	}


	template<typename T>
	Vector3<T>& Vector3<T>::operator/= (T d)
	{
		X /= d;
		Y /= d;
		Z /= d;
		return *this;
	}


	template<typename T>
	float Vector3<T>::length() const
	{
		return static_cast<float>(sqrt(X * X + Y * Y + Z * Z));
	}


	template<typename T>
	float Vector3<T>::distance(const Vector3<T>& v) const
	{
		return static_cast<float>(sqrt((X - v.X) * (X - v.X) + (Y - v.Y) * (Y - v.Y) + (Z - v.Z) * (Z - v.Z)));
	}

	template<typename T>
	float Vector3<T>::squareDistance(const Vector3<T>& v) const
	{
		return static_cast<float>((X - v.X) * (X - v.X) + (Y - v.Y) * (Y - v.Y) + (Z - v.Z) * (Z - v.Z));
	}
}
