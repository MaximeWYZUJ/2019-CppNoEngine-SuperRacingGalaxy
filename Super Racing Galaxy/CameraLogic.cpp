#include "pch.h"
#include "CameraLogic.h"
#include "SceneManager.h"

using namespace std;
using namespace Cookie;

CameraLogic::CameraLogic(SceneManager& sceneManager)
	: sceneManager(sceneManager),
	firstCurRotY(0.0f), firstCurRotX(0.0f),
	thirdCurRotY(0.0f), thirdCurRotX(0.0f), thirdTargetRotY(0.0f), thirdTargetRotX(0.0f), thirdTargetDistance(1.0f), thirdRotSmooth(0.8f),
	curUp(0.0f, 1.0f, 0.0f), curForward(0.0f, 0.0f, 1.0f)
{
	CreateCameras();
}

std::pair<float, float> CameraLogic::ThirdGetRotations() const noexcept
{
	return { thirdTargetRotY, thirdTargetRotX };
}

void CameraLogic::ThirdSetRotations(float rotY, float rotX)
{
	if (rotX >= Math::Pi / 2.01f)
	{
		rotX = Math::Pi / 2.01f;
	}
	if (rotX <= -Math::Pi / 2.01f)
	{
		rotX = -Math::Pi / 2.01f;
	}

	thirdTargetRotY = rotY;
	thirdTargetRotX = rotX;
}

void CameraLogic::ThirdSetDistance(float distance)
{
	thirdTargetDistance = distance;
}

void CameraLogic::Update(Vector3<> const& up, Vector3<> const& forward, Vector3<> const& center, float smooth)
{
	if (activeCameraType == CameraType::FirstPerson)
	{
		// Todo:
	}
	else if (activeCameraType == CameraType::ThirdPerson)
	{
		thirdCam->SetUpVector(up);
		Vector3<> right = Vector3<>::CrossProduct(up, forward);

		Vector3<> back(0.0f, 0.0f, -1.0f);

		// Rotate on Y, then rotate on right vector resulting from first rotation
		Quaternion<> targetRotY = Quaternion<>::FromDirection(thirdTargetRotY, { 0.0f, 1.0f, 0.0f });
		Vector4<> alignedRight = Vector4<>::CrossProduct(Matrix4x4<>::FromRotation(targetRotY) * back, Vector3<>(0.0f, 1.0f, 0.0f));
		alignedRight.Normalize();
		Quaternion targetRotX = Quaternion<>::FromDirection(thirdTargetRotX, alignedRight);
		Vector3<> targetPos = targetRotX * targetRotY * back * thirdTargetDistance;
		targetPos = right * targetPos.x + up * targetPos.y + forward * targetPos.z;
		targetPos += center;

		Vector3<> curCamPos = thirdCam->sceneNode->localTransform.GetPosition();

		targetPos = Vector3<>::Lerp(curCamPos, targetPos, smooth);

		thirdCam->sceneNode->localTransform.SetPosition(targetPos);

		Vector3<> front(0.0f, 0.0f, 1.0f);
		Quaternion<> camRot = Quaternion<>::FromVectorToVector(front, Vector3<>::Normalize(center - targetPos));
		thirdCam->sceneNode->localTransform.SetRotation(camRot);
	}
}

void CameraLogic::CreateCameras()
{
	SceneNode* root = sceneManager.GetRoot();

	SceneNode* thirdCamNode = sceneManager.AddSceneNode(root);
	thirdCam = sceneManager.AddCamera(thirdCamNode);

	SceneNode* firstCamNode = sceneManager.AddSceneNode(root);
	firstCam = sceneManager.AddCamera(firstCamNode);

	SetActiveCamera(CameraType::ThirdPerson);
}

void CameraLogic::SetActiveCamera(CameraType cameraType)
{
	switch (cameraType)
	{
	case CameraType::FirstPerson :
		sceneManager.SetMainCamera(firstCam);
		break;
	case CameraType::ThirdPerson:
		sceneManager.SetMainCamera(thirdCam);
		break;
	default:
		throw exception("Invalid enum value");
	}

	activeCameraType = cameraType;
}