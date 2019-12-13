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

	void Camera::SetUpVector(Vector3<> const& up)
	{
		this->up = up;
	}
	
	void Camera::UpdateMatrices()
	{
		if (isProjectionDirty)
		{
			projection = Matrix4x4<>::FromPerspectiveLH(fieldOfView, aspectRatio, nearPlane, farPlane);
			isProjectionDirty = false;
		}

		Vector3<> eyePosition = Vector3<>(sceneNode->matrix._14, sceneNode->matrix._24, sceneNode->matrix._34);
		Quaternion rot = sceneNode->localTransform.GetRotation();
		Vector4<> forward = rot * Vector3<>(0.0f, 0.0f, 1.0f);
		Vector3<> focusPosition = eyePosition + forward;
		view = Matrix4x4<>::FromLookAtLH(eyePosition, focusPosition, up);
		
		projView = projection * view;
	}
}
