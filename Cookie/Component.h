#pragma once

#include "ExportMacro.h"
#include "SceneNode.h"

namespace Cookie
{
	struct COOKIE_API Component
	{
		SceneNode* Parent;
		Transform* Transform;
	};
}
