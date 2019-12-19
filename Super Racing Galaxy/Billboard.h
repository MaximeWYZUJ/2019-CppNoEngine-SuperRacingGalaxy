#pragma once
#include "Prefab.h"

class Billboard : Prefab
{
public:
	Billboard(Cookie::Transform<> transform, std::string meshPath, std::wstring texturePath);

	void Update();
};