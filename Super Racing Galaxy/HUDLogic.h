#pragma once
#include "CameraLogic.h"
#include "Vector3.h"
#include "Vector4.h"
#include <vector>
#include "ActionManager.h"
#include "Scenario.h"
#include "Engine.h"

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
	Cookie::Engine* engine;

	Gdiplus::Font* font;
	Gdiplus::Font* font2;
	Gdiplus::Font* font3;
	
	Cookie::Vector3<> vehicleForward;
	Cookie::Vector3<> vehicleRight;
	Cookie::Vector4<> vehicleUp;
	
	std::vector<std::string> menuState;
	Cookie::ActionManager::StateType oldState;

	Cookie::Sprite* avantVitessometre;
	Cookie::Sprite* fondVitessometre;
	Cookie::Sprite* aiguilleVitessometre;
	Cookie::Text* speedCounter;
	Cookie::Sprite* timeBackground;
	Cookie::Text* timeCounter;
	bool ultraSpeed = false;
	
	Cookie::Sprite* fondMenu;
	Cookie::Sprite* effetVitesse;
	Cookie::Button* exit;
	Cookie::Sprite* playDuFond;
	Cookie::Button* play;
	
	Cookie::Sprite* pauseText;
	Cookie::Button* reprendre;
	
	Cookie::Sprite* victoryText;
	Cookie::Button* mainMenuButton;

	Scenario scenario;

	Gdiplus::FontFamily* pFontFamily;
	Gdiplus::PrivateFontCollection privateFontCollection;
	WCHAR familyName[LF_FACESIZE] = {};
	
	int timer = 0;
	
	int seconds = 0;
	int minutes = 0;
	int speed = 0;
	
	HUDType actualHUD;

	void createInGameContext();
	void createFont();
public:
	
	HUDLogic(Cookie::GuiManager* guiManager, Cookie::ActionManager* actionManager, CameraLogic& cameraLogic, Scenario& scenario, Cookie::Engine* engine);
	
	void setActiveHUD(HUDType hudType);

	HUDType getActiveHUD() const;

	void Update();
	
};