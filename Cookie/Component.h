#pragma once

#include "ExportMacro.h"
#include "SceneNode.h"

namespace Cookie
{
	class COOKIE_API Component
	{
	public:
		SceneNode* sceneNode;
		Matrix4x4<>* matrix;
	};
}
