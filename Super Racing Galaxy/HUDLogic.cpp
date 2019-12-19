#include "pch.h"
#include "HUDLogic.h"
#include "ActionManager.h"
#include "GuiManager.h"
#include "Button.h"
#include "ActionDescriptor.h"
#include "Vehicle.h"
#include "SceneManager.h"
#include "Planet.h"
#include "Cargo.h"

#undef max

using namespace Cookie;

HUDLogic::HUDLogic(GuiManager* guiManager, ActionManager* actionManager, CameraLogic& cameraLogic, Scenario& scenario, Cookie::Engine* engine) :
	guiManager{ guiManager },
	actionManager{ actionManager },
	cameraLogic{ cameraLogic },
	engine{engine},
	avantVitessometre{ nullptr },
	aiguilleVitessometre{ nullptr },
	fondVitessometre{ nullptr },
	speedCounter{ nullptr },
	timeBackground{ nullptr },
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
	createFont();
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
			auto rot = Quaternion<>::FromDirection(-Math::Pi / 45.0f, vehicleUp);
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
			auto rot = Quaternion<>::FromDirection(Math::Pi / 45.0f, vehicleUp);
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
		[]() {})),
		ActionDescriptor(Key::E, StateType::Pressed, std::chrono::milliseconds(0), std::chrono::milliseconds(0), ActionDescriptor::Callbacks(
		[]() {},
		[this]()
		{
			if (scenario.vehicle->mayUseImpulse()) {

				Planet* closestPlanet = nullptr;

				Vehicle* vehicle = scenario.vehicle;
				Vector3<> vehiclePos = vehicle->root->localTransform.GetPosition();

				float distanceMin = std::numeric_limits<float>::max();
				for (auto& planet : scenario.gravityGenerators)
				{
					auto planetPos = planet->root->localTransform.GetPosition();
					auto planetRadius = planet->root->localTransform.GetScale().x / 2;
					auto distance = Vector3<>::Distance(vehiclePos, planetPos) - planetRadius;

					if (distance < distanceMin)
					{
						distanceMin = distance;
						closestPlanet = planet;
					}
				}

				auto cargoDistance = Vector3<>::Distance(vehiclePos, scenario.cargo->root->localTransform.GetPosition());
				bool isCargoClosest = cargoDistance < distanceMin;

				auto gravityDir = Vector3<>(0.0f, 1.0f, 0.0f);
				if (!isCargoClosest)
				{
					gravityDir = vehiclePos - closestPlanet->gravityCenter;
					gravityDir.Normalize();
				}
				
				scenario.vehicle->root->localTransform.SetRotation(Quaternion<>::FromVectorToVector(Vector3<>::Forward(), gravityDir));
				auto vPos = scenario.vehicle->root->localTransform.GetPosition();
				auto direction = vPos - scenario.vehicle->gravityApplied;
				direction.Normalize();
				scenario.vehicle->root->physics->addImpulse(direction * 40);
				scenario.vehicle->root->localTransform.SetPosition(vPos + direction * 0.5);

				scenario.vehicle->root->physics->isDirty = true;
				scenario.vehicle->useImpulse();
			}
		},
		[]() {}))
		});
}

void HUDLogic::createFont()
{
	int count = 0;
	int found = 0;
	// Add two font files to the private collection.
	privateFontCollection.AddFontFile(L"graphics/fonts/Ash.ttf");
	privateFontCollection.AddFontFile(L"graphics/fonts/digital-7.ttf");

	// How many font families are in the private collection?
	count = privateFontCollection.GetFamilyCount();

	// Allocate a buffer to hold the array of FontFamily objects returned by GetFamilies.
	pFontFamily = new Gdiplus::FontFamily[count];

	// Get the array of FontFamily objects.
	privateFontCollection.GetFamilies(count, pFontFamily, &found);

	for (int j = 0; j < count; ++j)
	{
		// Get the font family name.
		pFontFamily[j].GetFamilyName(familyName);
	}
}


void HUDLogic::setActiveHUD(HUDType hudType)
{
	pFontFamily[0].GetFamilyName(familyName);
	font = new Gdiplus::Font(familyName, 12.0f, Gdiplus::FontStyleBold, Gdiplus::UnitPixel, &privateFontCollection);
	font3 = new Gdiplus::Font(familyName, 12.0f, Gdiplus::FontStyleItalic, Gdiplus::UnitPixel, &privateFontCollection);
	pFontFamily[1].GetFamilyName(familyName);
	font2 = new Gdiplus::Font(familyName, 40.0f, Gdiplus::FontStyleBold, Gdiplus::UnitPixel, &privateFontCollection);

	switch (hudType)
	{
	case HUDType::MainMenuHUD :
	{
		// Main Menu
		if(oldState.empty())
		{
			engine->pauseGameSwitch();
			oldState = actionManager->GetState();
		}
		scenario.vehicle->root->localTransform = scenario.vehicle->initialTransform;
		scenario.vehicle->root->physics->isDirty = true;
		scenario.vehicle->root->physics->resetAcceleration = true;
		actionManager->SetState(menuState);
		speedCounter = nullptr;
		timeCounter = nullptr;
		timer = 0;
		seconds = 0;
		minutes = 0;
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
		play = guiManager->newButton("graphics/sprite/dds/boutonPlay2.dds", "graphics/sprite/dds/boutonPlay2.dds", widthButton, heightButton, font, L"", guiManager->ScreenWidth / 2 - widthButton/2, guiManager->ScreenHeight * 3 / 4 - heightButton / 2, [this]()
			{
				guiManager->deleteGuiElement(fondMenu);
				guiManager->deleteGuiElement(play);
				guiManager->deleteGuiElement(playDuFond);
				guiManager->deleteGuiElement(exit);
				guiManager->deleteGuiElement(effetVitesse);
				cameraLogic.SetActiveCamera(CameraType::ThirdPerson);
				setActiveHUD(HUDType::InGameHUD);
			}, Gdiplus::Color(255, 255 ,255, 255), 0, 0,-angleRotBouton, Vector3{0.0f, 0.0f, 1.0f}, false, angleRotBouton);
		
		actualHUD = HUDType::MainMenuHUD;
		break;
	}
		
	case HUDType::InGameHUD :
	{
		engine->pauseGameSwitch();
		actionManager->SetState(oldState);
		int widthVitessometre = 200;
		int heightVitessometre = 200;
		if(fondVitessometre == nullptr)
			fondVitessometre = guiManager->newSprite("graphics/sprite/dds/fondVitessometre.dds", 0, 0, 1, 1, widthVitessometre, heightVitessometre);
		if (aiguilleVitessometre == nullptr)
			aiguilleVitessometre = guiManager->newSprite("graphics/sprite/dds/aiguilleVitessometre.dds", 0, 0, 1, 1, widthVitessometre, heightVitessometre, Math::Pi/6, Vector3<>{0.0f, 0.0f, 1.0f}, true);
		if (avantVitessometre == nullptr)
		{
			avantVitessometre = guiManager->newSprite("graphics/sprite/dds/avantVitessometre.dds", 0, 0, 1, 1, widthVitessometre, heightVitessometre);
			guiManager->addSwapTextureSprite("graphics/sprite/dds/avantBleuVitessometre.dds", avantVitessometre, true, [this]()
				{
				
					float vehicleSpeed = scenario.vehicle->root->physics->velocity.Length();
					if(vehicleSpeed * 180 / 142 > 99 && !ultraSpeed)
					{
						guiManager->changeTextColor(speedCounter, Gdiplus::Color(255, 0, 255, 255));
						guiManager->changeTextFont(speedCounter, font3);
						ultraSpeed = true;
						return true;
					} if(vehicleSpeed * 180 / 142 < 99 && ultraSpeed)
					{
						guiManager->changeTextColor(speedCounter, Gdiplus::Color(255, 255, 255, 255));
						guiManager->changeTextFont(speedCounter, font);
						ultraSpeed = false;
						return true;
					}
					return false;
					
				});
		}
			
		
		if (speedCounter == nullptr)
			speedCounter = guiManager->newText(60, 50, font, L" 000 %", 50 + widthVitessometre / 2 - 60 / 2, 50 + widthVitessometre * 3 / 5);
		if(timeBackground == nullptr)
			timeBackground = guiManager->newSprite("graphics/sprite/dds/fondChrono.dds", (guiManager->ScreenWidth - 120) / 2, 0, 1, 1, 125, 50);
		if (timeCounter == nullptr)
			timeCounter = guiManager->newText(120, 50, font2, L" 00 : 00", (guiManager->ScreenWidth - 120) / 2, 5, Gdiplus::Color(255, 0, 255, 0));
		actualHUD = HUDType::InGameHUD;
		break;
	}
		
	case HUDType::PauseMenuHUD :
	{
		// Pause Menu
		engine->pauseGameSwitch();
		oldState = actionManager->GetState();
		actionManager->SetState(menuState);
		float widthText = 1048 * guiManager->ScreenWidth / 1920;
		float heightText = 256 * guiManager->ScreenHeight / 1080;
		float widthBouton = 500 * guiManager->ScreenWidth / 1920;
		float heightBouton = 300 * guiManager->ScreenHeight / 1080;
		pauseText = guiManager->newSprite("graphics/sprite/dds/pause.dds", (guiManager->ScreenWidth - widthText) / 2, guiManager->ScreenHeight/3 - heightText / 2, 1, 1, widthText, heightText);
		reprendre = guiManager->newButton("graphics/sprite/dds/boutonReprendre2.dds", "graphics/sprite/dds/boutonReprendre1.dds", widthBouton, heightBouton, font, L"", guiManager->ScreenWidth * 2/8 - widthBouton / 2, guiManager->ScreenHeight * 3/4 - heightBouton, [this]()
			{
				guiManager->deleteGuiElement(mainMenuButton);
				guiManager->deleteGuiElement(pauseText);
				guiManager->deleteGuiElement(reprendre);
				setActiveHUD(HUDType::InGameHUD);
			});
		mainMenuButton = guiManager->newButton("graphics/sprite/dds/boutonRetourMenu2.dds", "graphics/sprite/dds/boutonRetourMenu1.dds", widthBouton, heightBouton, font, L"", guiManager->ScreenWidth * 6 / 8 - widthBouton / 2, guiManager->ScreenHeight * 3 / 4 - heightBouton, [this]()
			{
				guiManager->deleteGuiElement(pauseText);
				guiManager->deleteGuiElement(reprendre);
				guiManager->deleteGuiElement(mainMenuButton);
				guiManager->deleteGuiElement(speedCounter);
				guiManager->deleteGuiElement(timeBackground);
				timeBackground = nullptr;
				guiManager->deleteGuiElement(timeCounter);
				guiManager->deleteGuiElement(avantVitessometre);
				avantVitessometre = nullptr;
				guiManager->deleteGuiElement(aiguilleVitessometre);
				aiguilleVitessometre = nullptr;
				guiManager->deleteGuiElement(fondVitessometre);
				fondVitessometre = nullptr;
				setActiveHUD(HUDType::MainMenuHUD);
			});
		actualHUD = HUDType::PauseMenuHUD;
		break;
	}
		
	case HUDType::EndMenuHUD :
	{
		engine->pauseGameSwitch();
		oldState = actionManager->GetState();
		actionManager->SetState(menuState);
		float widthText = 920 * guiManager->ScreenWidth / 1920;
		float heightText = 132 * guiManager->ScreenHeight / 1080;
		float widthBouton = 500 * guiManager->ScreenWidth / 1920;
		float heightBouton = 300 * guiManager->ScreenHeight / 1080;
			
		// End Menu
		victoryText = guiManager->newSprite("graphics/sprite/dds/victoire.dds", (guiManager->ScreenWidth - widthText) / 2, guiManager->ScreenHeight / 3 - heightText / 2, 1, 1, widthText, heightText);
		mainMenuButton = guiManager->newButton("graphics/sprite/dds/boutonRetourMenu2.dds", "graphics/sprite/dds/boutonRetourMenu1.dds", widthBouton, heightBouton, font, L"", guiManager->ScreenWidth / 2 - widthBouton / 2, guiManager->ScreenHeight * 3 / 4 - heightBouton, [this]()
			{
				guiManager->deleteGuiElement(victoryText);
				guiManager->deleteGuiElement(mainMenuButton);
				guiManager->deleteGuiElement(speedCounter);
				guiManager->deleteGuiElement(timeBackground);
				timeBackground = nullptr;
				guiManager->deleteGuiElement(timeCounter);
				guiManager->deleteGuiElement(avantVitessometre);
				avantVitessometre = nullptr;
				guiManager->deleteGuiElement(aiguilleVitessometre);
				aiguilleVitessometre = nullptr;
				guiManager->deleteGuiElement(fondVitessometre);
				fondVitessometre = nullptr;
				setActiveHUD(HUDType::MainMenuHUD);
			});
		actualHUD = HUDType::EndMenuHUD;
		break;
	}
	}	
}

HUDType HUDLogic::getActiveHUD() const {
	return actualHUD;
}

void HUDLogic::Update()
{
	auto rot = Matrix4x4<>::FromRotation(scenario.vehicle->root->localTransform.GetRotation());
	vehicleForward = rot * Vector3<>{ 0.0f, 0.0f, 1.0f };
	vehicleRight = rot * Vector3<>{ 1.0f, 0.0f, 0.0f };
	vehicleUp = Vector4<>::CrossProduct(vehicleForward, vehicleRight);
	vehicleUp.Normalize();
	float vehicleSpeed = scenario.vehicle->root->physics->velocity.Length();
	if(aiguilleVitessometre != nullptr && vehicleSpeed * 180 / 14162 < 1)
		aiguilleVitessometre->rotAngleMin = Math::Pi / 6 - 5 * Math::Pi / 6 * vehicleSpeed * 180 / 14162;
	
	if(actualHUD == HUDType::InGameHUD)
	{
		std::wstring fill;
		std::wstring fill2;

		if (vehicleSpeed * 180 / 142 <= 100.4f)
			guiManager->Write(fill + std::to_wstring(static_cast<int>(round(vehicleSpeed * 180 / 142))) + L" %", speedCounter);
		
		if (timer == 60)
		{
			++seconds;
			timer = 0;
		}	
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


