#pragma once

#include "Scenery.h"

class Landing : public Scenery
{
public:
	Landing(Cookie::Transform<> transform, std::string meshPath, std::wstring texturePath);
};

