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
		T r11, r21, r31, r41;
		T r12, r22, r32, r42;
		T r13, r23, r33, r43;
		T r14, r24, r34, r44;

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
		m.r11 = static_cast<T>(1);
		m.r12 = static_cast<T>(0);
		m.r13 = static_cast<T>(0);
		m.r14 = static_cast<T>(0);
		
		m.r21 = static_cast<T>(0);
		m.r22 = static_cast<T>(1);
		m.r23 = static_cast<T>(0);
		m.r24 = static_cast<T>(0);
		
		m.r31 = static_cast<T>(0);
		m.r32 = static_cast<T>(0);
		m.r33 = static_cast<T>(1);
		m.r34 = static_cast<T>(0);
		
		m.r41 = static_cast<T>(0);
		m.r42 = static_cast<T>(0);
		m.r43 = static_cast<T>(0);
		m.r44 = static_cast<T>(1);
		return m;
	}

	template<class T>
	Matrix4x4<T> Matrix4x4<T>::FromScaling(Vector3<T> s)
	{
		Matrix4x4<T> m;
		
		m.r11 = s.x;
		m.r12 = static_cast<T>(0);
		m.r13 = static_cast<T>(0);
		m.r14 = static_cast<T>(0);
		
		m.r21 = static_cast<T>(0);
		m.r22 = s.y;
		m.r23 = static_cast<T>(0);
		m.r24 = static_cast<T>(0);
		
		m.r31 = static_cast<T>(0);
		m.r32 = static_cast<T>(0);
		m.r33 = s.z;
		m.r34 = static_cast<T>(0);
		
		m.r41 = static_cast<T>(0);
		m.r42 = static_cast<T>(0);
		m.r43 = static_cast<T>(0);
		m.r44 = static_cast<T>(1);
		
		return m;
	}

	template<class T>
	Matrix4x4<T> Matrix4x4<T>::FromRotation(Quaternion<T> r)
	{
		Matrix4x4<T> m;

		m.r11 = r.x;
		m.r12 = -r.y;
		m.r13 = -r.z;
		m.r14 = -r.w;
		
		m.r21 = r.y;
		m.r22 = r.x;
		m.r23 = -r.w;
		m.r24 = r.z;
		
		m.r31 = r.z;
		m.r32 = r.w;
		m.r33 = r.x;
		m.r34 = -r.y;

		m.r41 = r.w;
		m.r42 = -r.z;
		m.r43 = r.y;
		m.r44 = r.x;
		
		return m;
	}

	template<class T>
	Matrix4x4<T> Matrix4x4<T>::FromTranslation(Vector3<T> t)
	{
		Matrix4x4<T> m = Identity();
		m.r14 = t.x;
		m.r24 = t.y;
		m.r34 = t.z;
		return m;
	}

	template<class T>
	Matrix4x4<T> Matrix4x4<T>::FromTransform(Transform<T> const& t)
	{
		return
			FromScaling(t.scale) *
			FromRotation(t.rotation) *
			FromTranslation(t.pos);
	}

	template<class T>
	Matrix4x4<T> operator*(Matrix4x4<T> const& lhs, Matrix4x4<T> const& rhs)
	{
		Matrix4x4<T> r;

		r.r11 = lhs.r11 * rhs.r11 + lhs.r12 * rhs.r21 + lhs.r13 * rhs.r31 + lhs.r14 * rhs.r41;
		r.r12 = lhs.r11 * rhs.r12 + lhs.r12 * rhs.r22 + lhs.r13 * rhs.r32 + lhs.r14 * rhs.r42;
		r.r13 = lhs.r11 * rhs.r13 + lhs.r12 * rhs.r23 + lhs.r13 * rhs.r33 + lhs.r14 * rhs.r43;
		r.r14 = lhs.r11 * rhs.r14 + lhs.r12 * rhs.r24 + lhs.r13 * rhs.r34 + lhs.r14 * rhs.r44;

		r.r21 = lhs.r21 * rhs.r11 + lhs.r22 * rhs.r21 + lhs.r23 * rhs.r31 + lhs.r24 * rhs.r41;
		r.r22 = lhs.r21 * rhs.r12 + lhs.r22 * rhs.r22 + lhs.r23 * rhs.r32 + lhs.r24 * rhs.r42;
		r.r23 = lhs.r21 * rhs.r13 + lhs.r22 * rhs.r23 + lhs.r23 * rhs.r33 + lhs.r24 * rhs.r43;
		r.r24 = lhs.r21 * rhs.r14 + lhs.r22 * rhs.r24 + lhs.r23 * rhs.r34 + lhs.r24 * rhs.r44;

		r.r31 = lhs.r31 * rhs.r11 + lhs.r32 * rhs.r21 + lhs.r33 * rhs.r31 + lhs.r34 * rhs.r41;
		r.r32 = lhs.r31 * rhs.r12 + lhs.r32 * rhs.r22 + lhs.r33 * rhs.r32 + lhs.r34 * rhs.r42;
		r.r33 = lhs.r31 * rhs.r13 + lhs.r32 * rhs.r23 + lhs.r33 * rhs.r33 + lhs.r34 * rhs.r43;
		r.r34 = lhs.r31 * rhs.r14 + lhs.r32 * rhs.r24 + lhs.r33 * rhs.r34 + lhs.r34 * rhs.r44;

		r.r41 = lhs.r41 * rhs.r11 + lhs.r42 * rhs.r21 + lhs.r43 * rhs.r31 + lhs.r44 * rhs.r41;
		r.r42 = lhs.r41 * rhs.r12 + lhs.r42 * rhs.r22 + lhs.r43 * rhs.r32 + lhs.r44 * rhs.r42;
		r.r43 = lhs.r41 * rhs.r13 + lhs.r42 * rhs.r23 + lhs.r43 * rhs.r33 + lhs.r44 * rhs.r43;
		r.r44 = lhs.r41 * rhs.r14 + lhs.r42 * rhs.r24 + lhs.r43 * rhs.r34 + lhs.r44 * rhs.r44;
		
		return r;
	}
}
