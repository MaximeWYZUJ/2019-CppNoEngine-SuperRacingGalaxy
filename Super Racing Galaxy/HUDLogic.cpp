#include "pch.h"
#include "HUDLogic.h"
#include "ActionManager.h"
#include "GuiManager.h"
#include "Button.h"
#include "ActionDescriptor.h"
#include "Vehicle.h"
#include "SceneManager.h"

#undef max

using namespace Cookie;

HUDLogic::HUDLogic(GuiManager* guiManager, ActionManager* actionManager, CameraLogic& cameraLogic, Scenario& scenario) :
	guiManager{ guiManager },
	actionManager{ actionManager },
	cameraLogic{ cameraLogic },
	speedCounter{ nullptr },
	timeCounter{ nullptr },
	fondMenu{ nullptr },
	effetVitesse{ nullptr },
	exit{ nullptr },
	playDuFond{ nullptr },
	play{ nullptr },
	pauseText{ nullptr },
	reprendre{ nullptr },
	menuState{ "MenuContext" },
	victoryText{ nullptr },
	mainMenuButton{ nullptr },
	scenario{ scenario }
{
	actionManager->CreateContext("MenuContext", {});
	createInGameContext();
	actionManager->EnableContext("InGameContext");
}

void HUDLogic::createInGameContext()
{	
	actionManager->CreateContext("InGameContext", {
		ActionDescriptor(Key::Alpha1, StateType::Pressed, std::chrono::milliseconds(0), std::chrono::milliseconds(0), ActionDescriptor::Callbacks(
		[]() {},
		[this]()
		{
			this->cameraLogic.SetActiveCamera(CameraType::FirstPerson);
		},
		[]() {})),
		ActionDescriptor(Key::Alpha2, StateType::Pressed, std::chrono::milliseconds(0), std::chrono::milliseconds(0), ActionDescriptor::Callbacks(
		[]() {},
		[this]()
		{
			this->cameraLogic.SetActiveCamera(CameraType::ThirdPerson);
		},
		[]() {})),
		ActionDescriptor(Key::Alpha3, StateType::Pressed, std::chrono::milliseconds(0), std::chrono::milliseconds(0), ActionDescriptor::Callbacks(
		[]() {},
		[this]()
		{
			this->cameraLogic.SetActiveCamera(CameraType::FreeCam);
		},
		[]() {})),
		ActionDescriptor(Key::W, StateType::Pressed, std::chrono::milliseconds(0), std::chrono::milliseconds(0), ActionDescriptor::Callbacks(
		[]() {},
		[this]()
		{
			Vector3<> velocity = scenario.vehicle->root->physics->velocity;

				auto [_, projVelocity] = Vector3<>::Projection(velocity, vehicleForward);

				if (projVelocity.Length() < 100.0f)
				{
					scenario.vehicle->root->physics->addForce(vehicleForward * 50.0f);
					scenario.vehicle->root->physics->isDirty = true;
				}
		},
		[]() {})),
		ActionDescriptor(Key::A, StateType::Pressed, std::chrono::milliseconds(0), std::chrono::milliseconds(0), ActionDescriptor::Callbacks(
		[]() {},
		[this]()
		{
			auto rot = Quaternion<>::FromDirection(-Math::Pi / 90.0f, vehicleUp);
				scenario.vehicle->root->localTransform.SetRotation(rot * scenario.vehicle->root->localTransform.GetRotation());
				scenario.vehicle->root->physics->isDirty = true;
		},
		[]() {})),
		ActionDescriptor(Key::S, StateType::Pressed, std::chrono::milliseconds(0), std::chrono::milliseconds(0), ActionDescriptor::Callbacks(
		[]() {},
		[this]()
		{
			scenario.vehicle->root->physics->addForce(-vehicleForward * 50.0f);
			scenario.vehicle->root->physics->isDirty = true;
		},
		[]() {})),
		ActionDescriptor(Key::D, StateType::Pressed, std::chrono::milliseconds(0), std::chrono::milliseconds(0), ActionDescriptor::Callbacks(
		[]() {},
		[this]()
		{
			auto rot = Quaternion<>::FromDirection(Math::Pi / 90.0f, vehicleUp);
			scenario.vehicle->root->localTransform.SetRotation(rot * scenario.vehicle->root->localTransform.GetRotation());
			scenario.vehicle->root->physics->isDirty = true;
		},
		[]() {})),
		ActionDescriptor(Key::P, StateType::Pressed, std::chrono::milliseconds(0), std::chrono::milliseconds(std::numeric_limits<long long>::max()), ActionDescriptor::Callbacks(
		[]() {},
		[this]()
		{
			setActiveHUD(HUDType::PauseMenuHUD);
		},
		[]() {}))
		});
}

void HUDLogic::setActiveHUD(HUDType hudType)
{
	Gdiplus::Font* font = new Gdiplus::Font(new Gdiplus::FontFamily(L"Comic Sans MS", nullptr), 40.0f, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);

	switch (hudType)
	{
	case HUDType::MainMenuHUD :
	{
		// Main Menu
		if(oldState.empty())
			oldState = actionManager->GetState();
		actionManager->SetState(menuState);
		float widthButton = 500 * guiManager->ScreenWidth / 1920;
		float heightButton = 300 * guiManager->ScreenHeight / 1080;
		float angleRotBouton = 10.0f * Math::Pi / 180.0f;
		fondMenu = guiManager->newSprite("graphics/sprite/dds/mainMenu.dds", 0, 0, 1, 1, guiManager->ScreenWidth, guiManager->ScreenHeight);
		effetVitesse = guiManager->newSprite("graphics/sprite/dds/effetVitesse1.dds", 0, 0, 1, 1, guiManager->ScreenWidth, guiManager->ScreenHeight);
		guiManager->addSwapTextureSprite("graphics/sprite/dds/effetVitesse2.dds", effetVitesse);
		playDuFond = guiManager->newSprite("graphics/sprite/dds/boutonPlay1.dds", guiManager->ScreenWidth/2 - widthButton / 2, guiManager->ScreenHeight*3/4 - heightButton/2, 1, 1, widthButton, heightButton, -angleRotBouton, Vector3<>{0.0f, 0.0f, 1.0f}, false, angleRotBouton);
		exit = guiManager->newButton("graphics/sprite/dds/boutonQuitter1.dds", "graphics/sprite/dds/boutonQuitter2.dds", 90, 102, font, L"", guiManager->ScreenWidth * 9 / 10, (guiManager->ScreenHeight - 102) / 2, []()
			{
				std::exit(0);
			});
		play = guiManager->newButton("graphics/sprite/dds/boutonPlay2.dds", "graphics/sprite/dds/boutonPlay2.dds", widthButton, heightButton, font, L"", guiManager->ScreenWidth / 2 - widthButton/2, guiManager->ScreenHeight * 3 / 4 - heightButton / 2, [this, &font]()
			{
				guiManager->deleteGuiElement(fondMenu);
				guiManager->deleteGuiElement(play);
				guiManager->deleteGuiElement(playDuFond);
				guiManager->deleteGuiElement(exit);
				guiManager->deleteGuiElement(effetVitesse);
				cameraLogic.SetActiveCamera(CameraType::ThirdPerson);
				setActiveHUD(HUDType::InGameHUD);
			}, 0, 0, -angleRotBouton, Vector3{0.0f, 0.0f, 1.0f}, false, angleRotBouton);
		
		actualHUD = HUDType::MainMenuHUD;
		break;
	}
		
	case HUDType::InGameHUD :
		actionManager->SetState(oldState);
		if(speedCounter == nullptr)
			speedCounter = guiManager->newText(200, 50, font, L" 000 km/h", 0, 0);
		if(timeCounter == nullptr)
			timeCounter = guiManager->newText(200, 50, font, L" 00 : 00", (guiManager->ScreenWidth - 200) / 2, 0);
		actualHUD = HUDType::InGameHUD;
		break;
		
	case HUDType::PauseMenuHUD :
	{
		// Pause Menu
		oldState = actionManager->GetState();
		actionManager->SetState(menuState);
		float widthText = 1048 * guiManager->ScreenWidth / 1920;
		float heightText = 256 * guiManager->ScreenHeight / 1080;
		float widthBouton = 500 * guiManager->ScreenWidth / 1920;
		float heightBouton = 300 * guiManager->ScreenHeight / 1080;
		pauseText = guiManager->newSprite("graphics/sprite/dds/pause.dds", (guiManager->ScreenWidth - widthText) / 2, guiManager->ScreenHeight/3 - heightText / 2, 1, 1, widthText, heightText);
		reprendre = guiManager->newButton("graphics/sprite/dds/boutonReprendre1.dds", "graphics/sprite/dds/boutonReprendre2.dds", widthBouton, heightBouton, font, L"", guiManager->ScreenWidth * 2/8 - widthBouton / 2, guiManager->ScreenHeight * 3/4 - heightBouton, [this]()
			{
				guiManager->deleteGuiElement(mainMenuButton);
				guiManager->deleteGuiElement(pauseText);
				guiManager->deleteGuiElement(reprendre);
				setActiveHUD(HUDType::InGameHUD);
			});
		mainMenuButton = guiManager->newButton("graphics/sprite/dds/boutonRetourMenu1.dds", "graphics/sprite/dds/boutonRetourMenu2.dds", widthBouton, heightBouton, font, L"", guiManager->ScreenWidth * 6 / 8 - widthBouton / 2, guiManager->ScreenHeight * 3 / 4 - heightBouton, [this]()
			{
				guiManager->deleteGuiElement(pauseText);
				guiManager->deleteGuiElement(reprendre);
				guiManager->deleteGuiElement(mainMenuButton);
				guiManager->deleteGuiElement(speedCounter);
				guiManager->deleteGuiElement(timeCounter);
				setActiveHUD(HUDType::MainMenuHUD);
			});
		actualHUD = HUDType::PauseMenuHUD;
		break;
	}
		
	case HUDType::EndMenuHUD :
	{
		oldState = actionManager->GetState();
		actionManager->SetState(menuState);
		float widthText = 920 * guiManager->ScreenWidth / 1920;
		float heightText = 132 * guiManager->ScreenHeight / 1080;
		float widthBouton = 500 * guiManager->ScreenWidth / 1920;
		float heightBouton = 300 * guiManager->ScreenHeight / 1080;
			
		// End Menu
		victoryText = guiManager->newSprite("graphics/sprite/dds/victoire.dds", (guiManager->ScreenWidth - widthText) / 2, guiManager->ScreenHeight / 3 - heightText / 2, 1, 1, widthText, heightText);
		mainMenuButton = guiManager->newButton("graphics/sprite/dds/boutonRetourMenu1.dds", "graphics/sprite/dds/boutonRetourMenu2.dds", widthBouton, heightBouton, font, L"", guiManager->ScreenWidth / 2 - widthBouton / 2, guiManager->ScreenHeight * 3 / 4 - heightBouton, [this]()
			{
				guiManager->deleteGuiElement(victoryText);
				guiManager->deleteGuiElement(mainMenuButton);
				guiManager->deleteGuiElement(speedCounter);
				guiManager->deleteGuiElement(timeCounter);
				setActiveHUD(HUDType::MainMenuHUD);
			});
		actualHUD = HUDType::EndMenuHUD;
		break;
	}
	}	
}

void HUDLogic::Update(Vector3<> velocity)
{
	auto rot = Matrix4x4<>::FromRotation(scenario.vehicle->root->localTransform.GetRotation());
	vehicleForward = rot * Vector3<>{ 0.0f, 0.0f, 1.0f };
	vehicleRight = rot * Vector3<>{ 1.0f, 0.0f, 0.0f };
	vehicleUp = Vector4<>::CrossProduct(vehicleForward, vehicleRight);
	vehicleUp.Normalize();
	
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


