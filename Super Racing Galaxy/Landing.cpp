#include "Landing.h"

using namespace std;

Landing::Landing(Cookie::Transform<> transform, string meshPath, wstring texturePath)
		:Scenery(transform, meshPath, texturePath, meshPath)
{

}