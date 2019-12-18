#pragma once

#include "Vector3.h"
#include "Quaternion.h"

namespace Cookie
{
	template<class T = float>
	class Transform
	{
	public:
		static Transform<T> BlenderToCookie(Vector3<T> pos, Vector3<T> scale, Quaternion<T> rotation);
		static Transform<T> FromMatrix(Matrix4x4<T> m);
		
		Transform();
		Transform(Vector3<T> pos, Vector3<T> scale, Quaternion<T> rotation);

		void SetPosition(Vector3<T> pos_);
		void SetScale(Vector3<T> scale_);
		void SetRotation(Quaternion<T> rotation_);

		Vector3<T> const& GetPosition() const;
		Vector3<T> const& GetScale() const;
		Quaternion<T> const& GetRotation() const;
		
		bool IsDirty();
		void ResetDirty();
		void SetDirty();
		Vector3<T> GetGlobalPosition() const;
		Vector3<T> GetGlobalScale() const;
		Quaternion<T> GetGlobalRotation() const;
	private:
		Vector3<T> position;
		Vector3<T> scale;
		Quaternion<T> rotation;

		Vector3<T> globalPosition;
		Vector3<T> globalScale;
		Quaternion<T> globalRotation;

		bool isDirty;
	};

	template <class T>
	Transform<T> Transform<T>::BlenderToCookie(Vector3<T> pos, Vector3<T> scale, Quaternion<T> rotation)
	{
		return Transform<T>(Vector3<T>(pos.x, pos.z, pos.y), Vector3<T>(scale.x, scale.z, scale.y), Quaternion<T>(rotation.y, rotation.w, rotation.z, -rotation.x));
	}

	template<class T>
	Transform<T> Transform<T>::FromMatrix(Matrix4x4<T> m)
	{
		// Position
		Vector3<T> pos;
		pos.x = m._14;
		pos.y = m._24;
		pos.z = m._34;

		// Scale
		m._14 = m._24 = m._34 = 0.0f;
		Vector3<T> scale;
		scale.x = Vector3<T>(m._11, m._21, m._31).Length();
		scale.y = Vector3<T>(m._12, m._22, m._32).Length();
		scale.z = Vector3<T>(m._13, m._23, m._33).Length();

		// Rotation
		m._11 /= scale.x;
		m._21 /= scale.x;
		m._31 /= scale.x;

		m._12 /= scale.y;
		m._22 /= scale.y;
		m._32 /= scale.y;

		m._13 /= scale.z;
		m._23 /= scale.z;
		m._33 /= scale.z;

		Quaternion<T> rotation = Quaternion<T>::FromRotationMatrix(m);

		return Transform<T>(pos, scale, rotation);
	}

	template<class T>
	Transform<T>::Transform()
		: position(static_cast<T>(0), static_cast<T>(0), static_cast<T>(0)),
		  scale(static_cast<T>(1), static_cast<T>(1), static_cast<T>(1)),
		  rotation(static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(1)),
		  isDirty { false }
	{
	}

	template<class T>
	Transform<T>::Transform(Vector3<T> pos, Vector3<T> scale, Quaternion<T> rotation)
		: position(pos),
		scale(scale),
		rotation(rotation),
		isDirty(true)
	{
	}
	
	template<class T>
	void Transform<T>::SetPosition(Vector3<T> pos_)
	{
		position = pos_;
		isDirty = true;
	}
	
	template<class T>
	void Transform<T>::SetScale(Vector3<T> scale_)
	{
		scale = scale_;
		isDirty = true;
	}
	
	template<class T>
	void Transform<T>::SetRotation(Quaternion<T> rotation_)
	{
		rotation = rotation_;
		isDirty = true;
	}

	template<class T>
	Vector3<T> const& Transform<T>::GetPosition() const
	{
		return position;
	}

	template<class T>
	Vector3<T> const& Transform<T>::GetScale() const
	{
		return scale;
	}

	template<class T>
	Quaternion<T> const& Transform<T>::GetRotation() const
	{
		return rotation;
	}
	
	template<class T>
	bool Transform<T>::IsDirty()
	{
		return isDirty;
	}
	
	template<class T>
	void Transform<T>::ResetDirty()
	{
		isDirty = false;
	}
	
	template<class T>
	void Transform<T>::SetDirty()
	{
		isDirty = true;
	}

	template<class T>
	Vector3<T> Transform<T>::GetGlobalPosition() const
	{
		throw;
	}

	template<class T>
	Vector3<T> Transform<T>::GetGlobalScale() const
	{
		throw;
	}

	template<class T>
	Quaternion<T> Transform<T>::GetGlobalRotation() const
	{
		throw;
	}
}