#pragma once
#include "Vector3.h"

namespace Cookie
{
	class ActionManager;
	struct SceneNode;
	class SceneManager;
	class Camera;
}

enum class CameraType
{
	FirstPerson,
	ThirdPerson,
	FreeCam
};

class CameraLogic
{
public:
	CameraLogic(Cookie::SceneManager& sceneManager, Cookie::ActionManager& actionManager);

	void SetActiveCamera(CameraType cameraType);
	
	// First person
	std::pair<float, float> ThirdGetRotations() const noexcept;
	void ThirdSetRotations(float rotY, float rotX);
	void ThirdSetDistance(float distance);
	
	void FreeSetRotations(float rotY, float rotX);
	std::pair<float, float> FreeGetRotations();

	void Update(Cookie::Vector3<> const& up, Cookie::Vector3<> const& forward, Cookie::Vector3<> const& center, float smooth);
private:
	void CreateCameras();
	void RegisterCameraActions();

	Cookie::SceneManager& sceneManager;
	Cookie::ActionManager& actionManager;

	Cookie::Camera* firstCam;
	Cookie::Camera* thirdCam;
	Cookie::Camera* freeCam;

	float firstCurRotY;
	float firstCurRotX;

	float thirdCurRotY;
	float thirdCurRotX;
	float thirdTargetRotY;
	float thirdTargetRotX;
	float thirdTargetDistance;
	float thirdRotSmooth;

	Cookie::Vector3<> freeCurPos;
	float freeCurRotY;
	float freeCurRotX;
	float freeCurSpeed;
	int freePressedKeys;

	Cookie::Vector3<> curUp;
	Cookie::Vector3<> curForward;

	CameraType activeCameraType;
};
