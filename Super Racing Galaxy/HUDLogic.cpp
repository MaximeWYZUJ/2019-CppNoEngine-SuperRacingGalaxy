#include "pch.h"
#include "HUDLogic.h"
#include "ActionManager.h"
#include "GuiManager.h"
#include "Button.h"

HUDLogic::HUDLogic(Cookie::GuiManager* guiManager, Cookie::ActionManager* actionManager, CameraLogic& cameraLogic) :
guiManager{ guiManager },
actionManager{ actionManager },
cameraLogic{ cameraLogic },
speedCounter{ nullptr },
timeCounter{ nullptr },
fondMenu{ nullptr },
play{ nullptr },
pauseText{ nullptr },
reprendre{ nullptr },
victoryText{ nullptr },
mainMenuButton{ nullptr }
{
	actionManager->CreateContext("MenuContext", {});
}

void HUDLogic::setActiveHUD(HUDType hudType, bool& keyPushed)
{
	Gdiplus::Font* font = new Gdiplus::Font(new Gdiplus::FontFamily(L"Comic Sans MS", nullptr), 40.0f, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);

	switch (hudType)
	{
	case HUDType::MainMenuHUD :
	{
		// Main Menu
		fondMenu = guiManager->newSprite("UneTexture.dds", 0, guiManager->ScreenHeight, 1, 1, guiManager->ScreenWidth, guiManager->ScreenHeight);
		play = guiManager->newButton("fondBouton.dds", "fondBoutonOver.dds", 200, 50, font, L"PLAY", 100, 300, [this, &font, &keyPushed]()
			{
				guiManager->deleteGuiElement(fondMenu);
				guiManager->deleteGuiElement(play);
				cameraLogic.SetActiveCamera(CameraType::ThirdPerson);
				setActiveHUD(HUDType::InGameHUD, keyPushed);
			});
		actionManager->EnableContext("MenuContext");
		actualHUD = HUDType::MainMenuHUD;
		break;
	}
		
	case HUDType::InGameHUD :
		if(speedCounter == nullptr)
			speedCounter = guiManager->newText(200, 50, font, L" 000 km/h", 0, 50);
		if(timeCounter == nullptr)
			timeCounter = guiManager->newText(200, 50, font, L" 00 : 00", (guiManager->ScreenWidth - 200) / 2, 50);
		actualHUD = HUDType::InGameHUD;
		break;
		
	case HUDType::PauseMenuHUD :
	{
		// Pause Menu
		keyPushed = true;
		pauseText = guiManager->newText(220, 50, font, L"PAUSE", (guiManager->ScreenWidth - 220) / 2, 150);
		reprendre = guiManager->newButton("fondBouton.dds", "fondBoutonOver.dds", 200, 50, font, L"Reprendre", (guiManager->ScreenWidth - 200) / 2, (guiManager->ScreenHeight + 50) / 2, [this, &keyPushed]()
			{
				guiManager->deleteGuiElement(pauseText);
				guiManager->deleteGuiElement(reprendre);
				keyPushed = false;
				setActiveHUD(HUDType::InGameHUD, keyPushed);
			});
		actualHUD = HUDType::PauseMenuHUD;
		break;
	}
		
	case HUDType::EndMenuHUD :
	{
		// End Menu
		victoryText = guiManager->newText(220, 50, font, L"VICTOIRE", (guiManager->ScreenWidth - 220) / 2, 150);
		mainMenuButton = guiManager->newButton("fondBouton.dds", "fondBoutonOver.dds", 400, 50, font, L"Retour au menu principal", (guiManager->ScreenWidth - 400) / 2, (guiManager->ScreenHeight + 50) / 2, [this, &keyPushed]()
			{
				guiManager->deleteGuiElement(victoryText);
				guiManager->deleteGuiElement(mainMenuButton);
				guiManager->deleteGuiElement(speedCounter);
				guiManager->deleteGuiElement(timeCounter);
				setActiveHUD(HUDType::MainMenuHUD, keyPushed);
			});
		actualHUD = HUDType::EndMenuHUD;
		break;
	}
	}	
}

void HUDLogic::Update(Cookie::Vector3<> velocity)
{
	if(actualHUD == HUDType::InGameHUD)
	{
		std::wstring fill;
		std::wstring fill2;
		speed = round(velocity.Length() * 3.6);
		speed < 100 ? (speed < 10 ? fill = L"00" : fill = L"0") : fill = L"";
		guiManager->Write(fill + std::to_wstring(speed) + L" km/h", speedCounter);

		if (timer % 60 == 0)
			++seconds;
		if (seconds == 60)
		{
			++minutes;
			seconds = 0;
		}

		minutes < 10 ? fill = L"0" : fill = L"";
		seconds < 10 ? fill2 = L"0" : fill2 = L"";
		guiManager->Write(fill + std::to_wstring(minutes) + L" : " + fill2 + std::to_wstring(seconds), timeCounter);

		timer++;
	}
}


