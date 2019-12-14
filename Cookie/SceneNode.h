#pragma once

#include <vector>
#include "ExportMacro.h"
#include "Transform.h"
#include "Matrix4x4.h"

namespace Cookie
{
	class Component;
	class PhysicsComponent;
	
	class COOKIE_API SceneNode
	{
	public:
		using SceneNodePtr = SceneNode*;
		SceneNodePtr parent;
		Transform<> localTransform;
		Matrix4x4<> localMatrix;
		Matrix4x4<> matrix;
		std::vector<SceneNode*> children;
		std::vector<Component*> components;

		PhysicsComponent* physics;
	};
}
