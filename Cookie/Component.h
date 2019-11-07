#pragma once

#include "ExportMacro.h"
#include "SceneNode.h"

namespace Cookie
{
	class COOKIE_API Component
	{
	public:
		SceneNode* parent;
		Matrix4x4<>* matrix;
	};
}
