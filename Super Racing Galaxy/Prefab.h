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
		PLANET, SCENERY, VEHICLE, TELEPORT, NOTHING, SKYBOX, GOAL, BILLBOARD, CARGO
	};

	Type type_;

	Cookie::Transform<> initialTransform;

	Cookie::SceneNode *root;

	std::string meshPath_ = "";
	std::string hitBoxPath_ = "";
	std::wstring texturePath_;
	std::string triggerPath_ = "";

	Cookie::Mesh *mesh;
	Cookie::Mesh *hitBox;
	Cookie::Mesh *triggerMesh;
	Cookie::Texture *texture;
};