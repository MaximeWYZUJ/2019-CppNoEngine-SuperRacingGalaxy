#include "Scenery.h"

using namespace std;
using namespace Cookie;

Scenery::Scenery(Transform<> transform, string meshPath, wstring texturePath)
{
	initialTransform = transform;
	type_ = Type::SCENERY;
	meshPath_ = meshPath;
	texturePath_ = texturePath;
	mesh = nullptr;
	texture = nullptr;
}
