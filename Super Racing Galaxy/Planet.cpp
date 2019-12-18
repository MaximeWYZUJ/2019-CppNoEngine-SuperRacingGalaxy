#include "Planet.h"
#include "Scenery.h"
#include "Teleport.h"
#include "Goal.h"

using namespace std;
using namespace Cookie;

Planet::Planet(Transform<> transform, float gravityValue, bool isUpVectorDynamic,
	           string meshPath, wstring texturePath)
	  : gravityCenter(transform.GetPosition()), gravityValue(gravityValue), isUpVectorDynamic(isUpVectorDynamic)
{
	initialTransform = transform;
	type_ = Type::PLANET;
	meshPath_ = meshPath;
	texturePath_ = texturePath;
	mesh = nullptr;
	texture = nullptr;
	goal = nullptr;
}

void Planet::addElement(Scenery* scenery)
{
	if (!scenery) return;
	setElements.push_back(scenery);
}

void Planet::addTeleport(Teleport *teleport)
{
	if (!teleport) return;
	teleportElements.push_back(teleport);
}

void Planet::addGoal(Goal *_goal)
{
	if (!_goal) return;
	goal = _goal;
}

void Planet::anime()
{
	cout << "Ca tourne !" << endl;
}
