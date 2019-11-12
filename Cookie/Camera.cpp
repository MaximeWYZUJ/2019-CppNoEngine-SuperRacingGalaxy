#include "pch.h"
#include "Camera.h"

namespace Cookie
{
	Camera::Camera(float fieldOfView, float aspectRatio, float nearPlane, float farPlane)
		: fieldOfView(fieldOfView), aspectRatio(aspectRatio), nearPlane(nearPlane), farPlane(farPlane), isProjectionDirty(true)
	{
	}

	Matrix4x4<> const& Camera::GetProjView() const noexcept
	{
		return projView;
	}
	
	void Camera::SetFieldOfView(float fieldOfView_)
	{
		fieldOfView = fieldOfView_;
		isProjectionDirty = true;
	}
	
	void Camera::SetAspectRatio(float aspectRatio_)
	{
		aspectRatio = aspectRatio_;
		isProjectionDirty = true;
	}
	
	void Camera::SetNearPlane(float nearPlane_)
	{
		nearPlane = nearPlane_;
		isProjectionDirty = true;
	}
	
	void Camera::SetFarPlane(float farPlane_)
	{
		farPlane = farPlane_;
		isProjectionDirty = true;
	}
	
	void Camera::UpdateMatrices()
	{
		if (isProjectionDirty)
		{
			projection = Matrix4x4<>::FromPerspective(fieldOfView, aspectRatio, nearPlane, farPlane);
			isProjectionDirty = false;
		}

		Vector3<> eyePosition = Vector3<>(parent->matrix._14, parent->matrix._24, parent->matrix._34);
		Vector4<> forwardNoRot(0.0f, 0.0f, 1.0f, 1.0f);
		Quaternion curRotation = parent->localTransform.GetRotation();
		Vector4<> dir = Matrix4x4<>::FromRotation(curRotation) * forwardNoRot;
		Vector3<> focusPosition = eyePosition + dir;
		view = Matrix4x4<>::FromLookAt(eyePosition, focusPosition, Vector3<>(0.0f, 1.0f, 0.0f));

		projView = projection * view;
	}
}
