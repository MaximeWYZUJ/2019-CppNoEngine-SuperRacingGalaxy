#pragma once

#include "Transform.h"

namespace Cookie {
	class Component;
	class PhysicsComponent;
	class SceneNode;
	class Mesh;
	class Texture;
}

class Prefab
{
public:
	enum class Type {
		PLANET, SCENERY, VEHICLE, TELEPORT, NOTHING
	};

	Type type_;

	Cookie::Transform<> initialTransform;

	Cookie::SceneNode *root;

	std::string meshPath_;
	std::wstring texturePath_;

	Cookie::Mesh *mesh;
	Cookie::Texture *texture;
};