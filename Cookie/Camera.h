#pragma once

#include "ExportMacro.h"
#include "Component.h"

namespace Cookie
{
	class COOKIE_API Camera : public Component
	{
	public:
		Camera(float fieldOfView, float aspectRatio, float nearPlane, float farPlane);
		
		Matrix4x4<> const& GetView();
		Matrix4x4<> const& GetProjection();

		void SetFieldOfView(float fieldOfView);
		void SetAspectRatio(float aspectRatio);
		void SetNearPlane(float nearPlane);
		void SetFarPlane(float farPlane);

		void UpdateMatrices();
	private:
		float fieldOfView;
		float aspectRatio;
		float nearPlane;
		float farPlane;

		bool isProjectionDirty;
		
		Matrix4x4<> view;
		Matrix4x4<> projection;
	};
}
