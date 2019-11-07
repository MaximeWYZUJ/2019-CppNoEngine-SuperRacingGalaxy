#pragma once

#include "Vector3.h"
#include "Quaternion.h"
#include "Transform.h"
#include <corecrt_math_defines.h>

namespace Cookie
{
	// Column-Major Matrix4x4.
	template<class T = float>
	class Matrix4x4
	{
	public:
		// Transposed for memory layout (column major)
		T _11, _21, _31, _41;
		T _12, _22, _32, _42;
		T _13, _23, _33, _43;
		T _14, _24, _34, _44;

		// Standard notation
		// (11) (12) (13) (14)
		// (21) (22) (23) (24)
		// (31) (32) (33) (34)
		// (41) (42) (43) (44)
		
		// Translation example
		// (11) (12) (13) x
		// (21) (22) (23) y
		// (31) (32) (33) z
		// (41) (42) (43) (44)

		static Matrix4x4<T> Identity();
		static Matrix4x4<T> FromScaling(Vector3<T> s);
		static Matrix4x4<T> FromRotation(Quaternion<T> r);
		static Matrix4x4<T> FromTranslation(Vector3<T> t);
		
		static Matrix4x4<T> FromTransform(Transform<T> const& t);
		static Matrix4x4<T> FromLookAt(Vector3<T> eyePosition, Vector3<T> focusPosition, Vector3<T> upDirection);
		static Matrix4x4<T> FromPerspective(float fieldOfView, float aspectRatio, float nearPlane, float farPlane);
		static Matrix4x4<T> Transpose(Matrix4x4<> const& matrix);
	};

	template<class T>
	Matrix4x4<T> Matrix4x4<T>::Identity()
	{
		Matrix4x4<T> m;
		m._11 = static_cast<T>(1);
		m._12 = static_cast<T>(0);
		m._13 = static_cast<T>(0);
		m._14 = static_cast<T>(0);

		m._21 = static_cast<T>(0);
		m._22 = static_cast<T>(1);
		m._23 = static_cast<T>(0);
		m._24 = static_cast<T>(0);

		m._31 = static_cast<T>(0);
		m._32 = static_cast<T>(0);
		m._33 = static_cast<T>(1);
		m._34 = static_cast<T>(0);

		m._41 = static_cast<T>(0);
		m._42 = static_cast<T>(0);
		m._43 = static_cast<T>(0);
		m._44 = static_cast<T>(1);
		return m;
	}

	template<class T>
	Matrix4x4<T> Matrix4x4<T>::FromScaling(Vector3<T> s)
	{
		Matrix4x4<T> m;

		m._11 = s.x;
		m._12 = static_cast<T>(0);
		m._13 = static_cast<T>(0);
		m._14 = static_cast<T>(0);

		m._21 = static_cast<T>(0);
		m._22 = s.y;
		m._23 = static_cast<T>(0);
		m._24 = static_cast<T>(0);

		m._31 = static_cast<T>(0);
		m._32 = static_cast<T>(0);
		m._33 = s.z;
		m._34 = static_cast<T>(0);

		m._41 = static_cast<T>(0);
		m._42 = static_cast<T>(0);
		m._43 = static_cast<T>(0);
		m._44 = static_cast<T>(1);

		return m;
	}

	template<class T>
	Matrix4x4<T> Matrix4x4<T>::FromRotation(Quaternion<T> r)
	{
		Matrix4x4<T> m;

		m._11 = 1.0f - 2.0f * r.y * r.y - 2.0f * r.z * r.z;
		m._12 = 2.0f * r.x * r.y + 2.0f * r.z * r.w;
		m._13 = 2.0f * r.x * r.z - 2.0f * r.y * r.w;
		m._14 = 0.0f;

		m._21 = 2.0f * r.x * r.y - 2.0f * r.z * r.w;
		m._22 = 1.0f - 2.0f * r.x * r.x - 2.0f * r.z * r.z;
		m._23 = 2.0f * r.y * r.z + 2.0f * r.x * r.w;
		m._24 = 0.0f;

		m._31 = 2.0f * r.x * r.z + 2.0f * r.y * r.w;
		m._32 = 2.0f * r.y * r.z - 2.0f * r.x * r.w;
		m._33 = 1.0f - 2.0f * r.x * r.x - 2.0f * r.y * r.y;
		m._34 = 0.0f;

		m._41 = 0.0f;
		m._42 = 0.0f;
		m._43 = 0.0f;
		m._44 = 1.0f;

		return m;
	}

	template<class T>
	Matrix4x4<T> Matrix4x4<T>::FromTranslation(Vector3<T> t)
	{
		Matrix4x4<T> m = Identity();
		m._14 = t.x;
		m._24 = t.y;
		m._34 = t.z;
		return m;
	}

	template<class T>
	Matrix4x4<T> Matrix4x4<T>::FromTransform(Transform<T> const& t)
	{
		return
			FromTranslation(t.GetPosition()) *
			FromRotation(t.GetRotation()) *
			FromScaling(t.GetScale());
	}

	template<class T>
	Matrix4x4<T> Matrix4x4<T>::FromLookAt(Vector3<T> eyePosition, Vector3<T> focusPosition, Vector3<T> upDirection)
	{
		Vector3<T> eyeDirection = focusPosition - eyePosition;
		
		//assert(eyePosition != Vector3<>::Zero());
		/*assert(!XMVector3IsInfinite(EyeDirection));
		assert(!XMVector3Equal(UpDirection, XMVectorZero()));
		assert(!XMVector3IsInfinite(UpDirection));*/

		Vector3<> R2 = Vector3<>::Normalize(eyeDirection);

		Vector3<> R0 = Vector3<>::CrossProduct(upDirection, R2);
		R0 = Vector3<>::Normalize(R0);

		Vector3<> R1 = Vector3<>::CrossProduct(R2, R0);

		Vector3<> NegEyePosition = -eyePosition;

		float D0 = Vector3<>::DotProduct(R0, NegEyePosition);
		float D1 = Vector3<>::DotProduct(R1, NegEyePosition);
		float D2 = Vector3<>::DotProduct(R2, NegEyePosition);

		Matrix4x4<> M;
		M._11 = R0.x; M._12 = R0.y; M._13 = R0.z; M._14 = D0;
		M._21 = R1.x; M._22 = R1.y; M._23 = R1.z; M._24 = D1;
		M._31 = R2.x; M._32 = R2.y; M._33 = R2.z; M._34 = D2;
		M._41 = 0.0f; M._42 = 0.0f; M._43 = 0.0f; M._44 = 1.0f;

		return M;
	}

	template<class T>
	Matrix4x4<T> Matrix4x4<T>::FromPerspective(float fieldOfView, float aspectRatio, float nearPlane, float farPlane)
	{
		//
		// Calculate Sin/Cos
		//
		
		// Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
		float quotient = M_1_PI / 2 * fieldOfView / 2;
		if (fieldOfView >= 0.0f)
		{
			quotient = static_cast<float>(static_cast<int>(quotient + 0.5f));
		}
		else
		{
			quotient = static_cast<float>(static_cast<int>(quotient - 0.5f));
		}
		float y = fieldOfView / 2 - M_PI * 2 * quotient;

		// Map y to [-pi/2,pi/2] with sin(y) = sin(Value).
		float sign;
		if (y > M_PI_2)
		{
			y = M_PI - y;
			sign = -1.0f;
		}
		else if (y < -M_PI_2)
		{
			y = -M_PI - y;
			sign = -1.0f;
		}
		else
		{
			sign = +1.0f;
		}

		float y2 = y * y;

		// 11-degree minimax approximation
		float SinFov = (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;

		// 10-degree minimax approximation
		float p = ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f;
		float CosFov = sign * p;

		//
		// Build perspective matrix
		//
		
		/*assert(NearZ > 0.f && FarZ > 0.f);
		assert(!XMScalarNearEqual(FovAngleY, 0.0f, 0.00001f * 2.0f));
		assert(!XMScalarNearEqual(AspectRatio, 0.0f, 0.00001f));
		assert(!XMScalarNearEqual(FarZ, NearZ, 0.00001f));*/

		float Height = CosFov / SinFov;
		float Width = Height / aspectRatio;
		float fRange = farPlane / (farPlane - nearPlane);

		Matrix4x4<> M;
		M._11 = Width;
		M._21 = 0.0f;
		M._31 = 0.0f;
		M._41 = 0.0f;

		M._12 = 0.0f;
		M._22 = Height;
		M._32 = 0.0f;
		M._42 = 0.0f;

		M._13 = 0.0f;
		M._23 = 0.0f;
		M._33 = fRange;
		M._43 = 1.0f;

		M._14 = 0.0f;
		M._24 = 0.0f;
		M._34 = -fRange * nearPlane;
		M._44 = 0.0f;
		
		return M;
	}

	template<class T>
	Matrix4x4<T> Matrix4x4<T>::Transpose(Matrix4x4<> const& matrix)
	{
		Matrix4x4<> m;
		m._11 = matrix._11;
		m._12 = matrix._21;
		m._13 = matrix._31;
		m._14 = matrix._41;

		m._21 = matrix._12;
		m._22 = matrix._22;
		m._23 = matrix._32;
		m._24 = matrix._42;

		m._31 = matrix._13;
		m._32 = matrix._23;
		m._33 = matrix._33;
		m._34 = matrix._43;

		m._41 = matrix._14;
		m._42 = matrix._24;
		m._43 = matrix._34;
		m._44 = matrix._44;
		
		return m;
	}

	template<class T>
	Matrix4x4<T> operator*(Matrix4x4<T> const& lhs, Matrix4x4<T> const& rhs)
	{
		Matrix4x4<T> m;

		m._11 = lhs._11 * rhs._11 + lhs._12 * rhs._21 + lhs._13 * rhs._31 + lhs._14 * rhs._41;
		m._12 = lhs._11 * rhs._12 + lhs._12 * rhs._22 + lhs._13 * rhs._32 + lhs._14 * rhs._42;
		m._13 = lhs._11 * rhs._13 + lhs._12 * rhs._23 + lhs._13 * rhs._33 + lhs._14 * rhs._43;
		m._14 = lhs._11 * rhs._14 + lhs._12 * rhs._24 + lhs._13 * rhs._34 + lhs._14 * rhs._44;
		
		m._21 = lhs._21 * rhs._11 + lhs._22 * rhs._21 + lhs._23 * rhs._31 + lhs._24 * rhs._41;
		m._22 = lhs._21 * rhs._12 + lhs._22 * rhs._22 + lhs._23 * rhs._32 + lhs._24 * rhs._42;
		m._23 = lhs._21 * rhs._13 + lhs._22 * rhs._23 + lhs._23 * rhs._33 + lhs._24 * rhs._43;
		m._24 = lhs._21 * rhs._14 + lhs._22 * rhs._24 + lhs._23 * rhs._34 + lhs._24 * rhs._44;
		
		m._31 = lhs._31 * rhs._11 + lhs._32 * rhs._21 + lhs._33 * rhs._31 + lhs._34 * rhs._41;
		m._32 = lhs._31 * rhs._12 + lhs._32 * rhs._22 + lhs._33 * rhs._32 + lhs._34 * rhs._42;
		m._33 = lhs._31 * rhs._13 + lhs._32 * rhs._23 + lhs._33 * rhs._33 + lhs._34 * rhs._43;
		m._34 = lhs._31 * rhs._14 + lhs._32 * rhs._24 + lhs._33 * rhs._34 + lhs._34 * rhs._44;
		
		m._41 = lhs._41 * rhs._11 + lhs._42 * rhs._21 + lhs._43 * rhs._31 + lhs._44 * rhs._41;
		m._42 = lhs._41 * rhs._12 + lhs._42 * rhs._22 + lhs._43 * rhs._32 + lhs._44 * rhs._42;
		m._43 = lhs._41 * rhs._13 + lhs._42 * rhs._23 + lhs._43 * rhs._33 + lhs._44 * rhs._43;
		m._44 = lhs._41 * rhs._14 + lhs._42 * rhs._24 + lhs._43 * rhs._34 + lhs._44 * rhs._44;
		
		return m;
	}
}
