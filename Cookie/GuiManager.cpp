#include "pch.h"
#include "GuiManager.h"
#include "Sprite.h"
#include "resource.h"
#include "Util.h"
#include "DeviceD3D11.h"
#include "Text.h"


namespace Cookie
{

	ULONG_PTR GuiManager::token = 0;
	
	struct ShadersParams
	{
		Matrix4x4<> matWVP; // la matrice totale
	};

	// Definir l’organisation de notre sommet
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
		pConstantBuffer{ nullptr },
		pVertexLayout{ nullptr },
		pSamplerState{ nullptr },
		pVertexShader{ nullptr },
		pPixelShader{ nullptr }
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
		// Initialisation de l’effet
		InitShaders();

		// Initialisation du texte
		InitText();
	}

	GuiManager::~GuiManager()
	{
		DXRelacher(pConstantBuffer);
		DXRelacher(pSamplerState);
		DXRelacher(pVertexShader);
		DXRelacher(pPixelShader);
		DXRelacher(pVertexLayout);
		DXRelacher(pVertexBuffer);
		CloseText();
	}

	void GuiManager::SetPosDim(Sprite* sprite, int x_, int y_, int dx_, int dy_)
	{
		float x = static_cast<float>(x_);
		float y = static_cast<float>(y_);

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

		// Positions en coordonnées logiques
		// 0,0 pixel = -1,1 (haut a gauche ecran)
		float posX = x * 2.0f / device->GetWidth() -1.0f;
		float posY = 1.0f - y * 2.0f / device->GetHeight();

		sprite->matPosDim = Matrix4x4<>::FromTranslation(Vector3{ posX, posY, 0.0f }) * Matrix4x4<>::FromScaling(Vector3{ facteurX, facteurY, 1.0f });
	}

	void GuiManager::InitShaders()
	{
		ID3D11Device* pD3DDevice = static_cast<DeviceD3D11*>(device)->GetD3DDevice();
		ID3DBlob* pVSBlob = nullptr;

		DXEssayer(D3DCompileFromFile(L"SpriteVS.hlsl",
			nullptr, nullptr,
			"SpriteVS",
			"vs_5_0",
			D3DCOMPILE_ENABLE_STRICTNESS,
			0,
			&pVSBlob, nullptr), DXE_FICHIER_VS);

		DXEssayer(pD3DDevice->CreateVertexShader(pVSBlob->GetBufferPointer(),
			pVSBlob->GetBufferSize(),
			nullptr,
			&pVertexShader),
			DXE_CREATION_VS);

		pVertexLayout = nullptr;
		ID3D11Device* d = static_cast<DeviceD3D11*>(device)->GetD3DDevice();

		DXEssayer(d->CreateInputLayout(SommetSprite::layout,
			SommetSprite::numElements,
			pVSBlob->GetBufferPointer(),
			pVSBlob->GetBufferSize(),
			&pVertexLayout),
			DXE_CREATIONLAYOUT);
		pVSBlob->Release();

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));

		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(ShadersParams);
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;

		pD3DDevice->CreateBuffer(&bd, nullptr, &pConstantBuffer);

		ID3DBlob* pPSBlob = nullptr;
		DXEssayer(D3DCompileFromFile(L"SpritePS.hlsl",
			nullptr, nullptr,
			"SpritePS",
			"ps_5_0",
			D3DCOMPILE_ENABLE_STRICTNESS,
			0,
			&pPSBlob,
			nullptr), DXE_FICHIER_PS);

		DXEssayer(pD3DDevice->CreatePixelShader(pPSBlob->GetBufferPointer(),
			pPSBlob->GetBufferSize(),
			nullptr,
			&pPixelShader),
			DXE_CREATION_PS);

		pPSBlob->Release();

		// Initialisation des paramètres de sampling de la texture
		D3D11_SAMPLER_DESC samplerDesc;
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		// Création de l’état de sampling
		pD3DDevice->CreateSamplerState(&samplerDesc, &pSamplerState);
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


	void GuiManager::newSprite(const std::string& textureName, int xPos, int yPos, int xDim, int yDim )
	{
		if (textureName != "")
		{
			std::wstring ws(textureName.begin(), textureName.end());

			Sprite* sprite = new Sprite();
			sprite->pTextureD3D = textureManager->GetNewTexture(ws.c_str(), device)->GetD3DTexture();

			SetPosDim(sprite, xPos, yPos, xDim, yDim);

			sprites.push_back(sprite);
		}
	}

	Text* GuiManager::newText(int width, int height, Gdiplus::Font* font, const std::wstring& s, int xPos, int yPos)
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
		// Notez que la brosse aurait pu être passée
		// en paramètre pour plus de flexibilité
		text->pBrush = new Gdiplus::SolidBrush(Gdiplus ::Color(255, 255, 255, 255));

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

		Write(s, text);

		SetPosDim(text, xPos, yPos, 1, 1);

		sprites.push_back(text);

		return text;
	}

	void GuiManager::Write(const std::wstring& s, Text* text)
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
	
	void GuiManager::DrawAll()
	{
		dynamic_cast<DeviceD3D11*>(device)->ResetDepthBuffer();

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

			// input layout des sommets
			pImmediateContext->IASetInputLayout(pVertexLayout);

			// Initialiser et sélectionner les «constantes» de l’effet
			ShadersParams sp;
			sp.matWVP = sprite->matPosDim;
			pImmediateContext->UpdateSubresource(pConstantBuffer, 0, nullptr, &sp, 0, 0);

			// Set shaders et constant buffer
			pImmediateContext->VSSetShader(pVertexShader, nullptr, 0);
			pImmediateContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
			pImmediateContext->GSSetShader(nullptr, nullptr, 0);
			pImmediateContext->PSSetShader(pPixelShader, nullptr, 0);
			pImmediateContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
			pImmediateContext->PSSetSamplers(0, 1, &pSamplerState);

			//Activation texture
			pImmediateContext->PSSetShaderResources(0, 1, &sprite->pTextureD3D);

			// **** Rendu de l’objet
			device->EnableAlphaBlend();
			pImmediateContext->Draw(6, 0);
			device->DisableAlphaBlend();
		}
	}

	
}
