#include "pch.h"
#include "BillBoard.h"

namespace Cookie
{
	void Billboard::Update(Quaternion<> cameraRot)
	{
		sceneNode->localTransform.SetRotation(cameraRot);
	}
}
