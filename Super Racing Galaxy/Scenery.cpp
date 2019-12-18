#include "Scenery.h"

using namespace std;
using namespace Cookie;

Scenery::Scenery(Transform<> transform, string meshPath, wstring texturePath, string hitBoxPath)
{
	initialTransform = transform;
	type_ = Type::SCENERY;
	meshPath_ = meshPath;
	texturePath_ = texturePath;
	hitBoxPath_ = hitBoxPath;
	mesh = nullptr;
	texture = nullptr;
}
