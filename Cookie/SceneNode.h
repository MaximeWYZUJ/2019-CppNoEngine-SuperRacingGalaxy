#pragma once

#include <vector>
#include "ExportMacro.h"

class Component;

namespace Cookie
{
	class COOKIE_API SceneNode
	{
	public:
		// Todo: Don't use pointer, should use a handle because component may move in memory
		void AddComponent(Component* component);
		void RemoveComponent(Component* component);
		void AddChild(SceneNode* sceneNode);
		std::vector<SceneNode*> const& GetChildren();
		std::vector<Component*> const& GetComponents();
	private:
		std::vector<SceneNode*> children;
		std::vector<Component*> components;
	};
}