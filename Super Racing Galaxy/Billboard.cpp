#include "Billboard.h"

using namespace std;
using namespace Cookie;

Billboard::Billboard(Cookie::Transform<> transform, std::string meshPath, std::wstring texturePath)
{
	initialTransform = transform;
	type_ = Type::SKYBOX;
	meshPath_ = meshPath;
	texturePath_ = texturePath;
	mesh = nullptr;
	texture = nullptr;
}

