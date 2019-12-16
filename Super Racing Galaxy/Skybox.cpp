#include "Skybox.h"

using namespace std;
using namespace Cookie;

Skybox::Skybox(Transform<> transform, string meshPath, wstring texturePath)
{
	initialTransform = transform;
	type_ = Type::SKYBOX;
	meshPath_ = meshPath;
	texturePath_ = texturePath;
	mesh = nullptr;
	texture = nullptr;
}