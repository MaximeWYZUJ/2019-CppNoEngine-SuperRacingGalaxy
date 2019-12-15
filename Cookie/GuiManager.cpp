#include "pch.h"
#include "GuiManager.h"
#include "Sprite.h"
#include "resource.h"
#include "Util.h"
#include "DeviceD3D11.h"
#include "Text.h"
#include "SpriteParams.h"

namespace Cookie
{
	
	ULONG_PTR GuiManager::token = 0;

	// Definir l�organisation de notre sommet
	D3D11_INPUT_ELEMENT_DESC SommetSprite::layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	UINT SommetSprite::numElements = ARRAYSIZE(layout);

	SommetSprite GuiManager::sommets[6] = {
		SommetSprite(Vector3<>(0.0f, 0.0f, 0.0f), Vector2<>(0.0f, 1.0f)),
		SommetSprite(Vector3<>(0.0f, 1.0f, 0.0f), Vector2<>(0.0f, 0.0f)),
		SommetSprite(Vector3<>(1.0f, 1.0f, 0.0f), Vector2<>(1.0f, 0.0f)),
		SommetSprite(Vector3<>(0.0f, 0.0f, 0.0f), Vector2<>(0.0f, 1.0f)),
		SommetSprite(Vector3<>(1.0f, 1.0f, 0.0f), Vector2<>(1.0f, 0.0f)),
		SommetSprite(Vector3<>(1.0f, 0.0f, 0.0f), Vector2<>(1.0f, 1.0f))
	};

	GuiManager::GuiManager(TextureManager* textureManager, DeviceD3D11* device) :
		textureManager{ textureManager },
		device{ device },
		ScreenWidth{ device->GetWidth() },
		ScreenHeight{ device->GetHeight() },
		pVertexBuffer{ nullptr },
		shader{device, L"Sprite", sizeof SpriteParams, SommetSprite::layout, SommetSprite::numElements}
	{
		// SPRITES
		
		// Cr�ation du vertex buffer et copie des sommets
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

	void GuiManager::SetPosDim(Sprite* sprite, int x_, int y_, int dx_, int dy_)
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

		float dimX = static_cast<float>(desc.Width);
		float dimY = static_cast<float>(desc.Height);
		
		// Dimensions en facteur
		float facteurX = dimX * 2.0f / device->GetWidth() * static_cast<float>(dx_);
		float facteurY = dimY * 2.0f / device->GetHeight() * static_cast<float>(dy_);

		// Positions en coordonn�es logiques
		// 0,0 pixel = -1,1 (haut a gauche ecran)
		float posX = static_cast<float>(x_) * 2.0f / device->GetWidth() -1.0f;
		float posY = 1.0f - static_cast<float>(y_) * 2.0f / device->GetHeight();

		sprite->matPosDim = Matrix4x4<>::FromTranslation(Vector3{ posX, posY, 0.0f }) * Matrix4x4<>::FromScaling(Vector3{ facteurX, facteurY, 1.0f });
	}

	void GuiManager::setButtonPosDim(Sprite* sprite, int x_, int y_, int dx_, int dy_)
	{
		// Dimensions en facteur
		float facteurX = static_cast<float>(dx_) * 2.0f / device->GetWidth();
		float facteurY = static_cast<float>(dy_) * 2.0f / device->GetHeight();

		// Positions en coordonn�es logiques
		// 0,0 pixel = -1,1 (haut a gauche ecran)
		float posX = static_cast<float>(x_) * 2.0f / device->GetWidth() - 1.0f;
		float posY = 1.0f - static_cast<float>(y_) * 2.0f / device->GetHeight();

		sprite->matPosDim = Matrix4x4<>::FromTranslation(Vector3{ posX, posY, 0.0f }) * Matrix4x4<>::FromScaling(Vector3{ facteurX, facteurY, 1.0f });
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


	Sprite* GuiManager::newSprite(const std::string& textureName, int xPos, int yPos, int xDim, int yDim, bool bouton)
	{
		if (textureName != "")
		{
			std::wstring ws(textureName.begin(), textureName.end());

			Sprite* sprite = new Sprite();
			sprite->pTextureD3D = textureManager->GetNewTexture(ws.c_str(), device)->GetD3DTexture();

			if (bouton) {
				setButtonPosDim(sprite, xPos, yPos, xDim, yDim);
				sprite->bouton = true;
			} else SetPosDim(sprite, xPos, yPos, xDim, yDim);
			
			sprites.push_back(sprite);

			return sprite;
		}
	}

	Text* GuiManager::newText(int width, int height, Gdiplus::Font* font, const std::wstring& text_, int xPos, int yPos)
	{
		Text* text = new Text();
		text->TextWidth = width;
		text->TextHeight = height;
		text->pFont = font;
		text->pCharBitmap = new Gdiplus::Bitmap(text->TextWidth, text->TextHeight, PixelFormat32bppARGB);
		text->pCharGraphics = new Gdiplus::Graphics(text->pCharBitmap);

		// Param�tres de l�objet Graphics
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
		// Notez que la brosse aurait pu �tre pass�e
		// en param�tre pour plus de flexibilit�
		text->pBrush = new Gdiplus::SolidBrush(Gdiplus ::Color(255, 255, 255, 255));

		// On efface le bitmap (notez le NOIR TRANSPARENT...)
		text->pCharGraphics->Clear(Gdiplus ::Color(0, 0, 0, 0));

		// Nous pourrions ici �crire une valeur initiale sur le bitmap
		// std ::wstring s=L"Valeur initiale";
		// pCharGraphics->DrawString( s.c_str(), s.size(), pFont, PointF( 0.0f, 0.0f ), pBlackBrush.get() );
		
		// Acc�der aux bits du bitmap
		Gdiplus::BitmapData bmData;
		text->pCharBitmap->LockBits(&Gdiplus::Rect(0, 0, text->TextWidth, text->TextHeight), Gdiplus ::ImageLockModeRead, PixelFormat32bppARGB, &bmData);
		
		// Cr�ation d�une texture de m�me dimension sur la carte graphique
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

		// Cr�ation de la texture � partir des donn�es du bitmap
		DXEssayer(device-> GetD3DDevice()->CreateTexture2D(&texDesc, &data, &text->pTexture));

		// Cr�ation d�un � resourve view � pour acc�der facilement � la texture
		// (comme pour les sprites)
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		srvDesc.Texture2D.MostDetailedMip = 0;

		DXEssayer(device->GetD3DDevice()->CreateShaderResourceView(text->pTexture, &srvDesc, &text->pTextureD3D));

		text->pCharBitmap->UnlockBits(&bmData);

		Write(text_, text);

		SetPosDim(text, xPos, yPos, 1, 1);

		sprites.push_back(text);

		return text;
	}

	Text* GuiManager::newButton(int width, int height, Gdiplus::Font* pFont, const std::wstring& text_, int xPos, int yPos, int offsetTextBoutonX, int offsetTextBoutonY)
	{
		newSprite("fondBouton.dds", xPos, yPos, width, height, true);
		return newText(width, height, pFont, text_, xPos + offsetTextBoutonX, yPos + offsetTextBoutonY);
	}

	void GuiManager::deleteGuiElement(Sprite* sprite)
	{
		auto spriteIt = std::find(sprites.begin(), sprites.end(), sprite);
		if (spriteIt != sprites.end())
		{
			if (spriteIt != sprites.begin())
			{
				auto spritePrec = *(spriteIt - 1);
				if (spritePrec->bouton) {
					sprites.erase(spriteIt);
					sprites.erase(std::find(sprites.begin(), sprites.end(), spritePrec));
				} else sprites.erase(spriteIt);
			}
			else sprites.erase(spriteIt);
		}
	}
	
	void GuiManager::Write(const std::wstring& s, Text* text)
	{
		if(text != nullptr)
		{
			// Effacer
			text->pCharGraphics->Clear(Gdiplus::Color(0, 0, 0, 0));

			// �crire le nouveau texte
			text->pCharGraphics->DrawString(s.c_str(), static_cast<int>(s.size()), text->pFont, Gdiplus::PointF(0.0f, 0.0f), text->pBrush);

			// Transf�rer
			Gdiplus::BitmapData bmData;
			text->pCharBitmap->LockBits(&Gdiplus::Rect(0, 0, text->TextWidth, text->TextHeight), Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &bmData);

			ID3D11DeviceContext* pImmediateContext;
			dynamic_cast<DeviceD3D11*>(device)->GetD3DDevice()->GetImmediateContext(&pImmediateContext);

			pImmediateContext->UpdateSubresource(text->pTexture, 0, 0, bmData.Scan0, text->TextWidth * 4, 0);

			text->pCharBitmap->UnlockBits(&bmData);
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

			SpriteParams* sp = new SpriteParams;
			sp->matWVP = sprite->matPosDim;
			
			shader.Activate(sp, sprite->pTextureD3D);

			// **** Rendu de l�objet
			device->EnableAlphaBlend();
			pImmediateContext->Draw(6, 0);
			device->DisableAlphaBlend();
		}

		device->enableZBuffer();
	}

	
}
