#pragma once

#include <vector>
#include "ExportMacro.h"
#include "Transform.h"

namespace Cookie
{
	class Component;
	
	struct COOKIE_API SceneNode
	{
		using SceneNodePtr = SceneNode*;
		SceneNodePtr Parent;
		Transform Transform;
		std::vector<SceneNode*> Children;
		std::vector<Component*> Components;
	};
}