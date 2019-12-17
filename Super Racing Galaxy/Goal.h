#pragma once
#include "Prefab.h"
#include "HUDLogic.h"

class Goal : public Prefab
{
	HUDLogic* hud;
public:
	Goal(Cookie::Transform<> transform, std::string meshPath, std::wstring texturePath);

	void bindHUD(HUDLogic* hud);
	void showEndingScreen();
};

