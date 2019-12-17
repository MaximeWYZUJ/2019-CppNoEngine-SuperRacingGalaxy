#include "Goal.h"

Goal::Goal(Cookie::Transform<> transform, std::string meshPath, std::wstring texturePath)
{
	initialTransform = transform;
	type_ = Type::GOAL;
	meshPath_ = meshPath;
	texturePath_ = texturePath;
	mesh = nullptr;
	texture = nullptr;
}

void Goal::bindHUD(HUDLogic* hud_)
{
	hud = hud_;
}

void Goal::showEndingScreen()
{
	hud->setActiveHUD(HUDType::EndMenuHUD);
}
