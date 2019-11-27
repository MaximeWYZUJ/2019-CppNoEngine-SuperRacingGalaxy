#include "Scenery.h"

using namespace std;
using namespace Cookie;

Scenery::Scenery(Transform<> transform, string meshPath, wstring texturePath)
{
	transform_ = transform;
	type_ = SCENERY;
	meshPath_ = meshPath;
	texturePath_ = texturePath;
	mesh = nullptr;
	texture = nullptr;
}
