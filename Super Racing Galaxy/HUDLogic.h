#pragma once
#include "CameraLogic.h"
#include "Vector3.h"

namespace Cookie
{
	class GuiManager;
	class ActionManager;
	class Text;
	class Sprite;
	class Button;
}

enum class HUDType
{
	MainMenuHUD,
	InGameHUD,
	PauseMenuHUD,
	EndMenuHUD
};

class HUDLogic
{
	Cookie::GuiManager* guiManager;
	Cookie::ActionManager* actionManager;
	CameraLogic& cameraLogic;

	Cookie::Text* speedCounter;
	Cookie::Text* timeCounter;
	
	Cookie::Sprite* fondMenu;
	Cookie::Button* play;
	
	Cookie::Text* pauseText;
	Cookie::Button* reprendre;
	
	Cookie::Text* victoryText;
	Cookie::Button* mainMenuButton;

	
	int timer = 0;
	
	int seconds = 0;
	int minutes = 0;
	int speed = 0;
	
	HUDType actualHUD;

public:
	
	HUDLogic(Cookie::GuiManager* guiManager, Cookie::ActionManager* actionManager, CameraLogic& cameraLogic);
	
	void setActiveHUD(HUDType hudType, bool& keyPushed);

	void Update(Cookie::Vector3<>);
	
};