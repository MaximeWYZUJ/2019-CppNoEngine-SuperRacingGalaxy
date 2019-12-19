#include "pch.h"
#include "CameraLogic.h"
#include "SceneManager.h"
#include "ActionDescriptor.h"
#include "ActionManager.h"
#include "SceneNode.h"

using namespace std;
using namespace Cookie;

CameraLogic::CameraLogic(SceneManager& sceneManager, ActionManager& actionManager)
	: sceneManager(sceneManager), actionManager(actionManager),
	firstCurRotY(0.0f), firstCurRotX(0.0f),
	thirdCurRotY(0.0f), thirdCurRotX(0.0f), thirdTargetRotY(0.0f), thirdTargetRotX(0.0f), thirdTargetDistance(1.0f), thirdRotSmooth(0.8f),
	freeCurPos(0.0f, 0.0f, 0.0f), freeCurRotY(0.0f), freeCurRotX(0.0f), freeCurSpeed(1.0f), freePressedKeys(0),
	curUp(0.0f, 1.0f, 0.0f), curForward(0.0f, 0.0f, 1.0f)
{
	CreateCameras();
	RegisterCameraActions();
}

void CameraLogic::SetActiveCamera(CameraType cameraType)
{
	switch (cameraType)
	{
	case CameraType::FirstPerson:
		sceneManager.SetMainCamera(firstCam);
		actionManager.EnableContext("CameraLogic_FirstPerson");
		break;
	case CameraType::ThirdPerson:
		sceneManager.SetMainCamera(thirdCam);
		actionManager.EnableContext("CameraLogic_ThirdPerson");
		break;
	case CameraType::FreeCam:
		sceneManager.SetMainCamera(freeCam);
		actionManager.EnableContext("CameraLogic_Free");
		break;
	default:
		throw exception("Invalid enum value");
	}

	activeCameraType = cameraType;
}

Vector3<> CameraLogic::GetActivateCameraPosition() const noexcept
{
	switch(activeCameraType)
	{
	case CameraType::FirstPerson:
		return firstCam->sceneNode->localTransform.GetPosition();
	case CameraType::ThirdPerson:
		return thirdCam->sceneNode->localTransform.GetPosition();
	case CameraType::FreeCam:
		return freeCam->sceneNode->localTransform.GetPosition();
	default:
		throw exception();
	}
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

void CameraLogic::FreeSetRotations(float rotY, float rotX)
{
	if (rotX >= Math::Pi / 2.01f)
	{
		rotX = Math::Pi / 2.01f;
	}
	if (rotX <= -Math::Pi / 2.01f)
	{
		rotX = -Math::Pi / 2.01f;
	}
	
	freeCurRotY = rotY;
	freeCurRotX = rotX;
}

std::pair<float, float> CameraLogic::FreeGetRotations()
{
	return { thirdTargetRotY, thirdTargetRotX };
}

void CameraLogic::Update(Vector3<> const& up, Vector3<> const& forward, Transform<> const& shipTransform, float smooth)
{
	if (activeCameraType == CameraType::FirstPerson)
	{
		auto rot = Matrix4x4<>::FromRotation(shipTransform.GetRotation());
		Vector3<> vehicleForward = rot * Vector3<>{ 0.0f, 0.0f, 1.0f };
		Vector3<> vehicleRight = rot * Vector3<>{ 1.0f, 0.0f, 0.0f };
		Vector3<> vehicleUp = Vector3<>::CrossProduct(vehicleForward, vehicleRight);
		vehicleForward.Normalize();
		vehicleUp.Normalize();

		firstCam->SetUpVector(vehicleUp);
		firstCam->sceneNode->localTransform.SetPosition(shipTransform.GetPosition() + vehicleUp / 2.0);
		firstCam->sceneNode->localTransform.SetRotation(shipTransform.GetRotation());
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
		targetPos += shipTransform.GetPosition();

		Vector3<> curCamPos = thirdCam->sceneNode->localTransform.GetPosition();

		targetPos = Vector3<>::Lerp(curCamPos, targetPos, smooth);

		thirdCam->sceneNode->localTransform.SetPosition(targetPos);

		Vector3<> front(0.0f, 0.0f, 1.0f);
		Quaternion<> camRot = Quaternion<>::FromVectorToVector(front, Vector3<>::Normalize(shipTransform.GetPosition() -targetPos));
		thirdCam->sceneNode->localTransform.SetRotation(camRot);
	}
	else if (activeCameraType == CameraType::FreeCam)
	{
		freeCam->SetUpVector(curUp);

		Quaternion<> rotY = Quaternion<>::FromDirection(freeCurRotY, Vector3<>::Up());
		Quaternion<> rotX = Quaternion<>::FromDirection(freeCurRotX, rotY * Vector3<>::Forward());

		freeCam->sceneNode->localTransform.SetPosition(freeCurPos);
		Quaternion<> rotLerp = Quaternion<>::Slerp(freeCam->sceneNode->localTransform.GetRotation(), rotX * rotY, 0.2f);
		freeCam->sceneNode->localTransform.SetRotation(rotLerp);
	}
}

void CameraLogic::CreateCameras()
{
	SceneNode* root = sceneManager.GetRoot();

	SceneNode* firstCamNode = sceneManager.AddSceneNode(root);
	firstCam = sceneManager.AddCamera(firstCamNode);
	
	SceneNode* thirdCamNode = sceneManager.AddSceneNode(root);
	thirdCam = sceneManager.AddCamera(thirdCamNode);

	SceneNode* freeCamNode = sceneManager.AddSceneNode(root);
	freeCam = sceneManager.AddCamera(freeCamNode);

	SetActiveCamera(CameraType::ThirdPerson);
}

void CameraLogic::RegisterCameraActions()
{
	actionManager.CreateContext("CameraLogic_Free", {
		ActionDescriptor(Key::W, StateType::Pressed, chrono::milliseconds(0), chrono::milliseconds(0), ActionDescriptor::Callbacks(
			[]() {},
			[this]()
			{
				freePressedKeys |= 0x01;
				freeCurPos += curForward * freeCurSpeed;
				freeCurSpeed += 0.05f;
			},
			[this]()
			{
				freePressedKeys &= ~0x01;
				if (freePressedKeys == 0)
				{
					freeCurSpeed = 1.0f;
				}
			})),
		ActionDescriptor(Key::A, StateType::Pressed, chrono::milliseconds(0), chrono::milliseconds(0), ActionDescriptor::Callbacks(
			[](){},
			[this]()
			{
				freePressedKeys |= 0x02;
				Vector3<> const right = Vector3<>::CrossProduct(curUp, curForward);
				freeCurPos -= right  * freeCurSpeed;
				freeCurSpeed += 0.05f;
			},
			[this]()
			{
				freePressedKeys &= ~0x02;
				if (freePressedKeys == 0)
				{
					freeCurSpeed = 1.0f;
				}
			})),
		ActionDescriptor(Key::S, StateType::Pressed, chrono::milliseconds(0), chrono::milliseconds(0), ActionDescriptor::Callbacks(
			[]() {},
			[this]()
			{
				freePressedKeys |= 0x04;
				freeCurPos -= curForward * freeCurSpeed;
				freeCurSpeed += 0.05f;
			},
			[this]()
			{
				freePressedKeys &= ~0x04;
				if (freePressedKeys == 0)
				{
					freeCurSpeed = 1.0f;
				}
			})),
		ActionDescriptor(Key::D, StateType::Pressed, chrono::milliseconds(0), chrono::milliseconds(0), ActionDescriptor::Callbacks(
			[]() {},
			[this]()
			{
				freePressedKeys |= 0x08;
				Vector3<> const right = Vector3<>::CrossProduct(curUp, curForward);
				freeCurPos += right * freeCurSpeed;
				freeCurSpeed += 0.05f;
			},
			[this]()
			{
				freePressedKeys &= ~0x08;
				if (freePressedKeys == 0)
				{
					freeCurSpeed = 1.0f;
				}
			})),
		ActionDescriptor(Key::Q, StateType::Pressed, chrono::milliseconds(0), chrono::milliseconds(0), ActionDescriptor::Callbacks(
			[]() {},
			[this]()
			{
				freePressedKeys |= 0x10;
				freeCurPos += curUp * freeCurSpeed;
				freeCurSpeed += 0.05f;
			},
			[this]()
			{
				freePressedKeys &= ~0x10;
				if (freePressedKeys == 0)
				{
					freeCurSpeed = 1.0f;
				}
			})),
		ActionDescriptor(Key::E, StateType::Pressed, chrono::milliseconds(0), chrono::milliseconds(0), ActionDescriptor::Callbacks(
			[]() {},
			[this]()
			{
				freePressedKeys |= 0x20;
				freeCurPos -= curUp * freeCurSpeed;
				freeCurSpeed += 0.05f;
			},
			[this]()
			{
				freePressedKeys &= ~0x20;
				if (freePressedKeys == 0)
				{
					freeCurSpeed = 1.0f;
				}
			})),
	});
}
