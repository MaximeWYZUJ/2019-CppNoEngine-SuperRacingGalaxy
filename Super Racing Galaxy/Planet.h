#pragma once

#include "Prefab.h"

#include <vector>

class Scenery;
class Teleport;
class Goal;

class Planet : public Prefab {
public:
	Cookie::Vector3<> gravityCenter;
	float gravityValue;
	bool isUpVectorDynamic;
	std::vector<Scenery*> setElements;
	std::vector<Teleport *> teleportElements;
	Goal* goal;
	std::wstring texture2Path;
	std::wstring textureAlphaPath;
	Cookie::Texture* texture2;
	Cookie::Texture* textureAlpha;
	
	Cookie::SceneNode* billboard;
	std::wstring billboardTexturePath;
	Cookie::Texture* billboardTexture;

	Planet(Cookie::Transform<> transform, float gravityValue, bool isUpVectorDynamic,
		   std::string meshPath, std::wstring texture1Path, std::wstring texture2Path, std::wstring textureAlphaPath, std::wstring billboardTexturePath);

	void addElement(Scenery* scenery);
	void addTeleport(Teleport *scenery);
	void addGoal(Goal *goal);
	void anime();
};