#pragma once

#include "Prefab.h"

class Scenery : public Prefab {
public:
	Scenery(Cookie::Transform<> transform, std::string meshPath, std::wstring texturePath, std::string hitBoxPath);

protected:
	virtual void Polymorphism(){};
};