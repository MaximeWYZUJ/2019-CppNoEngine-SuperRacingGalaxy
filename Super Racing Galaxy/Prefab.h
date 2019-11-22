#pragma once

#include "Transform.h"

namespace Cookie {
	class SceneNode;
	class Mesh;
	class Texture;
}

class Prefab {
public:
	enum Type {
		PLANET, SCENERY, VEHICLE, NOTHING
	};

	Type type_;

	Cookie::Transform<> transform_;

	Cookie::SceneNode* root;

	std::string meshPath_;
	std::wstring texturePath_;

	Cookie::Mesh* mesh;
	Cookie::Texture* texture;
};
