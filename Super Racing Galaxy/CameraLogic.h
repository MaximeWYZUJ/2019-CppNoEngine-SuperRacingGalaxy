#pragma once
#include "Vector3.h"

namespace Cookie
{
	struct SceneNode;
	class SceneManager;
	class Camera;
}

enum class CameraType
{
	FirstPerson,
	ThirdPerson
};

class CameraLogic
{
public:
	CameraLogic(Cookie::SceneManager& sceneManager);

	// First person
	std::pair<float, float> ThirdGetRotations() const noexcept;
	void ThirdSetRotations(float rotY, float rotX);
	void ThirdSetDistance(float distance);

	void Update(Cookie::Vector3<> const& up, Cookie::Vector3<> const& forward, Cookie::Vector3<> const& center, float smooth);
private:
	void CreateCameras();
	void SetActiveCamera(CameraType cameraType);

	Cookie::SceneManager& sceneManager;

	Cookie::Camera* firstCam;
	Cookie::Camera* thirdCam;

	float firstCurRotY;
	float firstCurRotX;

	float thirdCurRotY;
	float thirdCurRotX;
	float thirdTargetRotY;
	float thirdTargetRotX;
	float thirdTargetDistance;
	float thirdRotSmooth;

	Cookie::Vector3<> curUp;
	Cookie::Vector3<> curForward;

	CameraType activeCameraType;
};
