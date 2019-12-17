#pragma once
#include "CameraLogic.h"
#include "Vector3.h"
#include <vector>
#include "ActionManager.h"

namespace Cookie
{
	class GuiManager;
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
	
	std::vector<std::string> menuState;
	Cookie::ActionManager::StateType oldState;
	
	Cookie::Text* speedCounter;
	Cookie::Text* timeCounter;
	
	Cookie::Sprite* fondMenu;
	Cookie::Sprite* effetVitesse;
	Cookie::Button* exit;
	Cookie::Sprite* playDuFond;
	Cookie::Button* play;
	
	Cookie::Sprite* pauseText;
	Cookie::Button* reprendre;
	
	Cookie::Sprite* victoryText;
	Cookie::Button* mainMenuButton;

	
	int timer = 0;
	
	int seconds = 0;
	int minutes = 0;
	int speed = 0;
	
	HUDType actualHUD;

public:
	
	HUDLogic(Cookie::GuiManager* guiManager, Cookie::ActionManager* actionManager, CameraLogic& cameraLogic);
	
	void setActiveHUD(HUDType hudType);

	void Update(Cookie::Vector3<>);
	
};