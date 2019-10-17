#pragma once

#include "ExportMacro.h"
#include "SceneNode.h"

namespace Cookie
{
	struct COOKIE_API Component
	{
		SceneNode* parent;
		Matrix4x4<>* matrix;
	};
}
