#include "pch.h"
#include "Camera.h"

namespace Cookie
{
	Camera::Camera(float fieldOfView, float aspectRatio, float nearPlane, float farPlane)
		: fieldOfView(fieldOfView), aspectRatio(aspectRatio), nearPlane(nearPlane), farPlane(farPlane), isProjectionDirty(false)
	{
	}
	
	Matrix4x4<> const& Camera::GetView()
	{
		return view;
	}
	
	Matrix4x4<> const& Camera::GetProjection()
	{
		return projection;
	}
	
	void Camera::SetFieldOfView(float fieldOfView)
	{
	}
	
	void Camera::SetAspectRatio(float aspectRatio)
	{
	}
	
	void Camera::SetNearPlane(float nearPlane)
	{
	}
	
	void Camera::SetFarPlane(float farPlane)
	{
	}
	
	void Camera::UpdateMatrices()
	{
	}
}
