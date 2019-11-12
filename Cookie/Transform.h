#pragma once

#include "Vector3.h"
#include "Quaternion.h"

namespace Cookie
{
	template<class T = float>
	class Transform
	{
	public:
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
	private:
		Vector3<T> position;
		Vector3<T> scale;
		Quaternion<T> rotation;
		bool isDirty;
	};

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
}