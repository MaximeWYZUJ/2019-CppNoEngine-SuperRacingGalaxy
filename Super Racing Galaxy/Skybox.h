#pragma once

#include "Prefab.h"

class Skybox : public Prefab {
public:
	Skybox(Cookie::Transform<> transform, std::string meshPath, std::wstring texturePath);

};