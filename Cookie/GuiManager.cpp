#include "pch.h"
#include "GuiManager.h"
#include "Sprite.h"
#include "resource.h"
#include "Util.h"
#include "DeviceD3D11.h"
#include "Text.h"
#include "Button.h"
#include "SpriteParams.h"
#include "InputManager.h"
#include <random>

namespace Cookie
{
	
	ULONG_PTR GuiManager::token = 0;

	// Definir l’organisation de notre sommet
	D3D11_INPUT_ELEMENT_DESC SommetSprite::layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	UINT SommetSprite::numElements = ARRAYSIZE(layout);

	SommetSprite GuiManager::sommets[6] = {
		SommetSprite(Vector3<>(-0.5f, -0.5f, 0.0f), Vector2<>(0.0f, 1.0f)),
		SommetSprite(Vector3<>(-0.5f, 0.5f, 0.0f), Vector2<>(0.0f, 0.0f)),
		SommetSprite(Vector3<>(0.5f, 0.5f, 0.0f), Vector2<>(1.0f, 0.0f)),
		SommetSprite(Vector3<>(-0.5f, -0.5f, 0.0f), Vector2<>(0.0f, 1.0f)),
		SommetSprite(Vector3<>(0.5f, 0.5f, 0.0f), Vector2<>(1.0f, 0.0f)),
		SommetSprite(Vector3<>(0.5f, -0.5f, 0.0f), Vector2<>(1.0f, 1.0f))
	};

	GuiManager::GuiManager(TextureManager* textureManager, InputManager* inputManager, DeviceD3D11* device) :
		textureManager{ textureManager },
		inputManager{inputManager},
		device{ device },
		ScreenWidth{ device->GetWidth() },
		ScreenHeight{ device->GetHeight() },
		pVertexBuffer{ nullptr },
		shader{device, L"Sprite", sizeof SpriteParams, SommetSprite::layout, SommetSprite::numElements}
	{
		// SPRITES
		
		// Création du vertex buffer et copie des sommets
		ID3D11Device* pD3DDevice = device->GetD3DDevice();

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));

		bd.Usage = D3D11_USAGE_IMMUTABLE;
		bd.ByteWidth = sizeof(sommets);
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = sommets;

		DXEssayer(pD3DDevice->CreateBuffer(&bd, &InitData, &pVertexBuffer), DXE_CREATIONVERTEXBUFFER);

		// Initialisation du texte
		InitText();
	}

	GuiManager::~GuiManager()
	{
		DXRelacher(pVertexBuffer);
		CloseText();
	}

	void GuiManager::SetPosDim(Sprite* sprite, int x_, int y_, int scaleX, int scaleY, int xDimPix, int yDimPix, Matrix4x4<> rotMat) const
	{
		// Obtenir la dimension de la texture
		ID3D11Resource* pResource;
		ID3D11Texture2D* pTextureInterface = 0;
		sprite->pTextureD3D->GetResource(&pResource);

		pResource->QueryInterface<ID3D11Texture2D>(&pTextureInterface);
		D3D11_TEXTURE2D_DESC desc;
		pTextureInterface->GetDesc(&desc);

		DXRelacher(pResource);
		DXRelacher(pTextureInterface);

		float dimX;
		float dimY;
		
		if (xDimPix == 0 && yDimPix == 0)
		{
			dimX = static_cast<float>(desc.Width);
			dimY = static_cast<float>(desc.Height);
		} else
		{
			dimX = static_cast<float>(xDimPix);
			dimY = static_cast<float>(yDimPix);
		}
		
		
		// Dimensions en facteur
		float facteurX = dimX * static_cast<float>(scaleX);
		float facteurY = dimY * static_cast<float>(scaleY);

		// Scale screen
		float ratioX = 2.0f / device->GetWidth();
		float ratioY = 2.0f / device->GetHeight();
		
		// Positions en coordonnées logiques
		// 0,0 pixel = -1,1 (haut a gauche ecran)
		float posX = (static_cast<float>(x_) + dimX/2) - device->GetWidth() / 2;
		float posY =(static_cast<float>(-y_) - dimY/2) + device->GetHeight() / 2;

		sprite->Translation = Matrix4x4<>::FromTranslation(Vector3{ posX, posY, 0.0f });
		sprite->Scale = Matrix4x4<>::FromScaling(Vector3{ facteurX, facteurY, 1.0f });
		sprite->ScaleScreen = Matrix4x4<>::FromScaling(Vector3{ ratioX, ratioY, 1.0f });
		
		sprite->matPosDim = sprite->ScaleScreen * sprite->Translation * rotMat * sprite->Scale;
	}

	void GuiManager::ActualizeRot(Sprite* sprite)
	{
		Vector3<> test = { 0.0f, 1.0f, 0.0f };
		sprite->matPosDim = sprite->ScaleScreen * sprite->Translation * Matrix4x4<>::FromRotation(Quaternion<>::FromDirection(sprite->actualRot, sprite->axe)) * sprite->Scale;
		auto test2 = sprite->matPosDim* test;
	}


	void GuiManager::InitText()
	{
		Gdiplus::GdiplusStartupInput startupInput(0, TRUE, TRUE);
		Gdiplus::GdiplusStartupOutput startupOutput;

		GdiplusStartup(&token, &startupInput, &startupOutput);
	}

	void GuiManager::CloseText()
	{
		Gdiplus::GdiplusShutdown(token);
	}


	Sprite* GuiManager::newSprite(const std::string& textureName, int xPos, int yPos, int xScale, int yScale, int xDimPix, int yDimPix, float angleRotMin, Vector3<> axeRot, bool rotStatic, float angleRotMax)
	{
		if (textureName != "")
		{
			std::wstring ws(textureName.begin(), textureName.end());

			Sprite* sprite = new Sprite();
			sprite->pTextureD3D = textureManager->GetNewTexture(ws.c_str(), device)->GetD3DTexture();

			if (angleRotMax != 0)
			{
				SetPosDim(sprite, xPos, yPos, xScale, yScale, xDimPix, yDimPix, Matrix4x4<>::FromRotation(Quaternion<>::FromDirection(angleRotMax, axeRot)));
				sprite->rotationType = Sprite::RotType::randomRot;
				sprite->rotAngleMax = angleRotMax;
			} else if (!rotStatic)
			{
				SetPosDim(sprite, xPos, yPos, xScale, yScale, xDimPix, yDimPix, Matrix4x4<>::FromRotation(Quaternion<>::FromDirection(angleRotMin, axeRot)));
				sprite->rotationType = Sprite::RotType::continuousRot;
			} else
			{
				SetPosDim(sprite, xPos, yPos, xScale, yScale, xDimPix, yDimPix, Matrix4x4<>::FromRotation(Quaternion<>::FromDirection(angleRotMin, axeRot)));
			}

			sprite->rotAngleMin = angleRotMin;
			sprite->actualRot = angleRotMin;
			sprite->axe = axeRot;
		
			sprites.push_back(sprite);

			return sprite;
		}
	}

	Text* GuiManager::newText(int width, int height, Gdiplus::Font* font, const std::wstring& text_, int xPos, int yPos, Gdiplus::Color color, float angleRotMin, Vector3<> axeRot, bool rotStatic, float angleRotMax)
	{
		Text* text = new Text();
		text->TextWidth = width;
		text->TextHeight = height;
		text->pFont = font;
		text->pCharBitmap = new Gdiplus::Bitmap(text->TextWidth, text->TextHeight, PixelFormat32bppARGB);
		text->pCharGraphics = new Gdiplus::Graphics(text->pCharBitmap);

		// Paramètres de l’objet Graphics
		text->pCharGraphics->SetCompositingMode(Gdiplus ::CompositingModeSourceOver);
		text->pCharGraphics->SetCompositingQuality(Gdiplus ::CompositingQualityHighSpeed);
		text->pCharGraphics->SetInterpolationMode(Gdiplus::InterpolationModeHighQuality);
		text->pCharGraphics->SetPixelOffsetMode(Gdiplus::PixelOffsetModeHighSpeed);
		text->pCharGraphics->SetSmoothingMode(Gdiplus::SmoothingModeNone);
		text->pCharGraphics->SetPageUnit(Gdiplus::UnitPixel);
		Gdiplus::TextRenderingHint hint = Gdiplus::TextRenderingHintAntiAlias;
		//TextRenderingHintSystemDefault;
		text->pCharGraphics->SetTextRenderingHint(hint);

		// Un brosse noire pour le remplissage
		text->pBrush = new Gdiplus::SolidBrush(color);

		// On efface le bitmap (notez le NOIR TRANSPARENT...)
		text->pCharGraphics->Clear(Gdiplus ::Color(0, 0, 0, 0));

		// Nous pourrions ici écrire une valeur initiale sur le bitmap
		// std ::wstring s=L"Valeur initiale";
		// pCharGraphics->DrawString( s.c_str(), s.size(), pFont, PointF( 0.0f, 0.0f ), pBlackBrush.get() );
		
		// Accéder aux bits du bitmap
		Gdiplus::BitmapData bmData;
		text->pCharBitmap->LockBits(&Gdiplus::Rect(0, 0, text->TextWidth, text->TextHeight), Gdiplus ::ImageLockModeRead, PixelFormat32bppARGB, &bmData);
		
		// Création d’une texture de même dimension sur la carte graphique
		D3D11_TEXTURE2D_DESC texDesc;
		texDesc.Width = text->TextWidth;
		texDesc.Height = text->TextHeight;
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 1;
		texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = bmData.Scan0;
		data.SysMemPitch = text->TextWidth * 4;
		data.SysMemSlicePitch = 0;

		// Création de la texture à partir des données du bitmap
		DXEssayer(device-> GetD3DDevice()->CreateTexture2D(&texDesc, &data, &text->pTexture));

		// Création d’un « resourve view » pour accéder facilement à la texture
		// (comme pour les sprites)
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		srvDesc.Texture2D.MostDetailedMip = 0;

		DXEssayer(device->GetD3DDevice()->CreateShaderResourceView(text->pTexture, &srvDesc, &text->pTextureD3D));

		text->pCharBitmap->UnlockBits(&bmData);

		Write(text_, text);

		if (angleRotMax != 0)
		{
			SetPosDim(text, xPos, yPos, 1, 1, 0, 0, Matrix4x4<>::FromRotation(Quaternion<>::FromDirection(angleRotMax, axeRot)));
			text->rotationType = Sprite::RotType::randomRot;
			text->rotAngleMax = angleRotMax;
		}
		else if (!rotStatic)
		{
			SetPosDim(text, xPos, yPos, 1, 1, 0, 0, Matrix4x4<>::FromRotation(Quaternion<>::FromDirection(angleRotMin, axeRot)));
			text->rotationType = Sprite::RotType::continuousRot;
		}
		else
		{
			SetPosDim(text, xPos, yPos, 1, 1, 0, 0, Matrix4x4<>::FromRotation(Quaternion<>::FromDirection(angleRotMin, axeRot)));
		}
		
		text->rotAngleMin = angleRotMin;
		text->actualRot = angleRotMax;
		text->axe = axeRot;
		
		sprites.push_back(text);

		return text;
	}

	Button* GuiManager::newButton(const std::string& textureBackgroung, const std::string& textureOver, int width, int height, Gdiplus::Font* pFont, const std::wstring& text_, int xPos, int yPos, std::function<void()> action, Gdiplus::Color color, int offsetTextBoutonX, int offsetTextBoutonY, float angleRotMin, Vector3<> axeRot, bool rotStatic, float angleRotMax)
	{
		if (textureBackgroung != "" && textureOver != "")
		{
			std::wstring wsOver(textureOver.begin(), textureOver.end());
			
			Button* button = new Button;
			button->background = newSprite(textureBackgroung, xPos, yPos, 1, 1, width, height, angleRotMin, axeRot, rotStatic, angleRotMax);
			button->pTextureOver = textureManager->GetNewTexture(wsOver, device)->GetD3DTexture();
			button->xMin = xPos;
			button->xMax = xPos + width;
			button->yMin = yPos;
			button->yMax = yPos + height;
			button->text = newText(width, height, pFont, text_, xPos + offsetTextBoutonX, yPos + offsetTextBoutonY, color, angleRotMin, axeRot, rotStatic, angleRotMax);
			button->OnClick(action);
			
			buttons.push_back(button);

			return button;
		}
	}

	void GuiManager::deleteGuiElement(Sprite* sprite)
	{
		auto buttonIt = std::find(buttons.begin(), buttons.end(), sprite);
		if(buttonIt != buttons.end())
		{
			Button* button = static_cast<Button*>(sprite);
			sprites.erase(std::find(sprites.begin(), sprites.end(), button->background));
			sprites.erase(std::find(sprites.begin(), sprites.end(), button->text));
			buttons.erase(buttonIt);
		} else
		{
			auto spriteIt = std::find(sprites.begin(), sprites.end(), sprite);
			if (spriteIt != sprites.end())
			{
				sprites.erase(spriteIt);
			}
		}
	}

	void GuiManager::addSwapTextureSprite(const std::string& textureName, Sprite* sprite, bool specialPredicate,std::function<bool()> predicate) const
	{
		if (textureName != "")
		{
			std::wstring ws(textureName.begin(), textureName.end());

			sprite->pTextureSwap = textureManager->GetNewTexture(ws, device)->GetD3DTexture();
			sprite->specialPredicate = specialPredicate;
			sprite->predicate = predicate;
		}
	}
	
	void GuiManager::changeSpriteTexture(const std::string& textureName, Sprite* sprite) const
	{
		if (textureName != "")
		{
			std::wstring ws(textureName.begin(), textureName.end());
			
			sprite->pTextureD3D = textureManager->GetNewTexture(ws, device)->GetD3DTexture();
		}
	}

	void GuiManager::changeButtonBackground(const std::string& textureName, Button* button) const
	{
		changeSpriteTexture(textureName, button->background);
	}
	
	void GuiManager::Write(const std::wstring& s, Text* text) const
	{
		if(text != nullptr)
		{
			// Effacer
			text->pCharGraphics->Clear(Gdiplus::Color(0, 0, 0, 0));

			// Écrire le nouveau texte
			text->pCharGraphics->DrawString(s.c_str(), static_cast<int>(s.size()), text->pFont, Gdiplus::PointF(0.0f, 0.0f), text->pBrush);

			// Transférer
			Gdiplus::BitmapData bmData;
			text->pCharBitmap->LockBits(&Gdiplus::Rect(0, 0, text->TextWidth, text->TextHeight), Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &bmData);

			ID3D11DeviceContext* pImmediateContext;
			dynamic_cast<DeviceD3D11*>(device)->GetD3DDevice()->GetImmediateContext(&pImmediateContext);

			pImmediateContext->UpdateSubresource(text->pTexture, 0, 0, bmData.Scan0, text->TextWidth * 4, 0);

			text->pCharBitmap->UnlockBits(&bmData);
		}
	}

	void GuiManager::Update()
	{
		frameNumber++;
		
		std::for_each(buttons.begin(), buttons.end(), [&](Button* button)
			{
				Vector2<int> mousePos = inputManager->GetMousePosition();
				bool mouseInside = mousePos.x > button->xMin&& mousePos.x < button->xMax && mousePos.y > button->yMin&& mousePos.y < button->yMax;

				if (!button->over && mouseInside)
				{
					button->MouseOver();
					button->over = true;
				} else if (button->over && !mouseInside)
				{
					button->MouseOver();
					button->over = false;
				}
				
				auto events = inputManager->GetEvents();

				auto it = std::find_if(events.begin(), events.end(), [](auto& e)
					{
						return e.type == InputEventType::MouseStateChanged && std::get<MouseStateChanged>(e.data).button == Mouse::LeftButton && std::get<MouseStateChanged>(e.data).position == 0x00;
					});

				if (it != events.end() && mouseInside)
				{
					button->Click();
				}			
			});
		
		
		for (Sprite* sprite : sprites)
		{
			switch (sprite->rotationType)
			{
			case Sprite::RotType::staticRot:
				sprite->actualRot = sprite->rotAngleMin;
				ActualizeRot(sprite);
				break;
				
			case Sprite::RotType::randomRot:
			{
				if (frameNumber == 10)
				{
					sprite->actualRot = std::uniform_real_distribution<float>{ sprite->rotAngleMin, sprite->rotAngleMax }(prng);
					ActualizeRot(sprite);
				}
				break;
			}

			case Sprite::RotType::continuousRot:
				sprite->actualRot += sprite->rotAngleMin;
				ActualizeRot(sprite);
				break;
			}
			if(frameNumber == 10 && sprite == sprites.back())
				frameNumber = 0;

			if(sprite->specialPredicate)
			{
				if (sprite->predicate())
					sprite->Swap();
			} else
			{
				if(frameNumber % 2 == 0)
					sprite->Swap();
			}
		}

		
	}
	
	void GuiManager::DrawAll()
	{
		device->disableZBuffer();

		for(auto sprite : sprites)
		{
			//Obtenir le contexte
			ID3D11DeviceContext* pImmediateContext;
			dynamic_cast<DeviceD3D11*>(device)->GetD3DDevice()->GetImmediateContext(&pImmediateContext);

			// Choisir la topologie des primitives
			pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			// Source des sommets
			UINT stride = sizeof(SommetSprite);
			const UINT offset = 0;
			pImmediateContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);

			SpriteParams sp;
			sp.matWVP = sprite->matPosDim;
			
			shader.Activate(&sp, { sprite->pTextureD3D });

			// **** Rendu de l’objet
			device->EnableAlphaBlend();
			pImmediateContext->Draw(6, 0);
			device->DisableAlphaBlend();
		}

		device->enableZBuffer();
	}

	
}
