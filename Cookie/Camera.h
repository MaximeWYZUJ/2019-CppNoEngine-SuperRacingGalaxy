#pragma once

#include "ExportMacro.h"
#include "Component.h"

namespace Cookie
{
	class COOKIE_API Camera : public Component
	{
	public:
		Camera(float fieldOfView, float aspectRatio, float nearPlane, float farPlane);
		
		Matrix4x4<> const& GetProjView() const noexcept;

		void SetFieldOfView(float fieldOfView);
		void SetAspectRatio(float aspectRatio);
		void SetNearPlane(float nearPlane);
		void SetFarPlane(float farPlane);
		void SetUpVector(Vector3<> const& up);

		void UpdateMatrices();
	private:
		float fieldOfView;
		float aspectRatio;
		float nearPlane;
		float farPlane;
		Vector3<> up;

		bool isProjectionDirty;
		
		Matrix4x4<> view;
		Matrix4x4<> projection;
		Matrix4x4<> projView;
	};
}
