#include "Planet.h"
#include "Scenery.h"

using namespace std;
using namespace Cookie;

Planet::Planet(Transform<> transform, float gravityValue,
			   Vector3<> rotDir, float rotSpeed,
	           string meshPath, wstring texturePath)
	  : gravityCenter(transform.GetPosition()), gravityValue(gravityValue), rotDir(rotDir), rotSpeed(rotSpeed)
{
	transform_ = transform;
	type_ = PLANET;
	meshPath_ = meshPath;
	texturePath_ = texturePath;
	mesh = nullptr;
	texture = nullptr;
}

void Planet::addElement(Scenery* scenery)
{
	if (!scenery) return;
	setElements.push_back(scenery);
}

void Planet::anime()
{

}
