#pragma once

#include "Vector3.h"
#include "Quaternion.h"
#include "Transform.h"

namespace Cookie
{
	template<class T = float>
	class Matrix4x4
	{
	public:
		T _11, _21, _31, _41;
		T _12, _22, _32, _42;
		T _13, _23, _33, _43;
		T _14, _24, _34, _44;

		static Matrix4x4<T> Identity();
		static Matrix4x4<T> FromScaling(Vector3<T> s);
		static Matrix4x4<T> FromRotation(Quaternion<T> r);
		static Matrix4x4<T> FromTranslation(Vector3<T> t);
		
		static Matrix4x4<T> FromTransform(Transform<T> const& t);
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
