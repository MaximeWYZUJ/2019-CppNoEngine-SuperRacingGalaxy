#include "pch.h"
#include "SceneNode.h"

namespace SmallEngine
{
	using namespace std;

	void SceneNode::AddComponent(Component* component)
	{
		components.push_back(component);
	}

	void SceneNode::RemoveComponent(Component* component)
	{
		// Todo:
	}

	void SceneNode::AddChild(SceneNode* sceneNode)
	{
		children.push_back(sceneNode);
	}

	vector<SceneNode*> const& SceneNode::GetChildren()
	{
		return children;
	}

	vector<Component*> const& SceneNode::GetComponents()
	{
		return components;
	}
}
