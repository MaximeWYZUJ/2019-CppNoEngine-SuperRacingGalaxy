#include "pch.h"
#include "PostEffectManager.h"
#include "util.h"
#include "resource.h"
#include "DeviceD3D11.h"
#include "ShaderParams.h"
#include "RadialBlurParams.h"
#include "ShakingParams.h"

namespace Cookie {
	// Definir l’organisation de notre sommet
	D3D11_INPUT_ELEMENT_DESC CSommetPanneauPE::layout[] = {
		{"POSITION", 0,DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"TEXCOORD", 0,DXGI_FORMAT_R32G32_FLOAT, 0,12,D3D11_INPUT_PER_VERTEX_DATA, 0} };

	UINT CSommetPanneauPE::numElements = ARRAYSIZE(layout);

	CSommetPanneauPE PostEffectManager::sommets[6] = {
		CSommetPanneauPE(Vector3<>(-1.0f, -1.0f, 0.0f), Vector2<>(0.0f, 1.0f)),
		CSommetPanneauPE(Vector3<>(-1.0f, 1.0f, 0.0f), Vector2<>(0.0f, 0.0f)),
		CSommetPanneauPE(Vector3<>(1.0f, 1.0f, 0.0f), Vector2<>(1.0f, 0.0f)),
		CSommetPanneauPE(Vector3<>(-1.0f, -1.0f, 0.0f), Vector2<>(0.0f, 1.0f)),
		CSommetPanneauPE(Vector3<>(1.0f, 1.0f, 0.0f), Vector2<>(1.0f, 0.0f)),
		CSommetPanneauPE(Vector3<>(1.0f, -1.0f, 0.0f), Vector2<>(1.0f, 1.0f)) };

	PostEffectManager::~PostEffectManager()
	{
		DXRelacher(pVertexBuffer);
		DXRelacher(pResourceView);
		DXRelacher(pRenderTargetView);
		DXRelacher(pTextureScene);
		DXRelacher(pDepthStencilView);
		DXRelacher(pDepthTexture);
	}

	PostEffectManager::PostEffectManager(DeviceD3D11* device) :
		device(device),
		pVertexBuffer(nullptr),
		shaderNUL{ {device, L"PostEffectNul", sizeof ShadersParams, CSommetPanneauPE::layout, static_cast<int32_t>(CSommetPanneauPE::numElements), false}, true },
		radialBlur{ {device, L"RadialBlur", sizeof RadialBlurParams, CSommetPanneauPE::layout, static_cast<int32_t>(CSommetPanneauPE::numElements)}, false },
		shaking{ {device, L"Shaking", sizeof ShakingParams, CSommetPanneauPE::layout, static_cast<int32_t>(CSommetPanneauPE::numElements)}, false }
	{
		ID3D11Device* d3dDevice = device->GetD3DDevice();
		
		// Création du vertex buffer et copie des sommets

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		
		bd.Usage = D3D11_USAGE_IMMUTABLE;
		bd.ByteWidth = sizeof(sommets);
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = sommets;

		DXEssayer(d3dDevice->CreateBuffer(&bd, &InitData, &pVertexBuffer), DXE_CREATIONVERTEXBUFFER);

		InitPostEffect();

	}

	void PostEffectManager::InitPostEffect()
	{
		D3D11_TEXTURE2D_DESC textureDesc;
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

		// Description de la texture
		ZeroMemory(&textureDesc, sizeof(textureDesc));
		
		// Cette texture sera utilisée comme cible de rendu et
		// comme ressource de shader
		textureDesc.Width = device->GetWidth();
		textureDesc.Height = device->GetHeight();
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET|D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		// Création de la texture
		device->GetD3DDevice()->CreateTexture2D(&textureDesc, nullptr, &pTextureScene);

		// VUE - Cible de rendu
		renderTargetViewDesc.Format = textureDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;

		// Création de la vue
		device->GetD3DDevice()->CreateRenderTargetView(pTextureScene, &renderTargetViewDesc, &pRenderTargetView);

		// VUE – Ressource de shader
		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;

		// Création de la vue
		device->GetD3DDevice()->CreateShaderResourceView(pTextureScene, &shaderResourceViewDesc, &pResourceView);

		// Au tour du tampon de profondeur
		D3D11_TEXTURE2D_DESC depthTextureDesc;
		ZeroMemory( &depthTextureDesc, sizeof( depthTextureDesc ) );
		depthTextureDesc.Width = device->GetWidth();
		depthTextureDesc.Height = device->GetHeight();
		depthTextureDesc.MipLevels = 1;
		depthTextureDesc.ArraySize = 1;
		depthTextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthTextureDesc.SampleDesc.Count = 1;
		depthTextureDesc.SampleDesc.Quality = 0;
		depthTextureDesc.Usage = D3D11_USAGE_DEFAULT;
		depthTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthTextureDesc.CPUAccessFlags = 0;
		depthTextureDesc.MiscFlags = 0;

		DXEssayer(device->GetD3DDevice()->CreateTexture2D(&depthTextureDesc, NULL, &pDepthTexture), DXE_ERREURCREATIONTEXTURE);

		// Création de la vue du tampon de profondeur (ou de stencil)
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSView;
		ZeroMemory( &descDSView, sizeof descDSView );
		descDSView.Format = depthTextureDesc.Format;
		descDSView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSView.Texture2D.MipSlice = 0;
		
		DXEssayer(device->GetD3DDevice()->CreateDepthStencilView( pDepthTexture, &descDSView, &pDepthStencilView ), DXE_ERREURCREATIONDEPTHSTENCILTARGET );
	}

	void PostEffectManager::BeginPostEffect()
	{
		// Prendre en note l’ancienne surface de rendu
		pOldRenderTargetView = device->GetRenderTargetView();

		// Prendre en note l’ancienne surface de tampon Z
		pOldDepthStencilView = device->GetDepthStencilView();

		// Utiliser la texture comme surface de rendu et le tampon de profondeur associé
		device->SetRenderTargetView(pRenderTargetView, pDepthStencilView);
	}

	void PostEffectManager::EndPostEffect()
	{
		// Restaurer l’ancienne surface de rendu et le tampon de profondeur associé
		device->SetRenderTargetView(pOldRenderTargetView, pOldDepthStencilView);
	}

	void PostEffectManager::activatePostEffect(PostEffectType postEffectType)
	{
		switch(postEffectType)
		{
		case PostEffectType::NUL :
			shaderNUL.second = true;
			break;
		case PostEffectType::RadialBlur :
			radialBlur.second = true;
			break;
		case PostEffectType::Shaking :
			shaking.second = true;
			break;
		}
	}
	
	void PostEffectManager::deactivatePostEffect(PostEffectType postEffectType)
	{
		switch (postEffectType)
		{
		case PostEffectType::NUL:
			shaderNUL.second = false;
			break;
		case PostEffectType::RadialBlur:
			radialBlur.second = false;
			break;
		case PostEffectType::Shaking:
			shaking.second = false;
			break;
		}
	}
	
	void PostEffectManager::Draw()
	{
		if (time == 6)
			time = 0;
		
		ShadersParams* sp = new ShadersParams;
		RadialBlurParams* rbp = new RadialBlurParams;
		rbp->distance = 0.03f;
		rbp->remplissage1 = 0;
		rbp->remplissage2 = 0;
		rbp->remplissage3 = 0;
		ShakingParams* shp = new ShakingParams;
		shp->temps = time;
		shp->remplissage1 = 0;
		shp->remplissage2 = 0;
		shp->remplissage3 = 0;
		
		// Obtenir le contexte
		ID3D11DeviceContext* pImmediateContext;
		dynamic_cast<DeviceD3D11*>(device)->GetD3DDevice()->GetImmediateContext(&pImmediateContext);
		
		// Choisir la topologie des primitives
		pImmediateContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

		// Source des sommets
		UINT stride = sizeof( CSommetPanneauPE );
		UINT offset = 0;
		pImmediateContext->IASetVertexBuffers( 0, 1, &pVertexBuffer, &stride, &offset );

		if(shaderNUL.second)
			shaderNUL.first.Activate(sp, pResourceView, false);
		if (shaking.second)
			shaking.first.Activate(shp, pResourceView);
		if (radialBlur.second)
			radialBlur.first.Activate(rbp, pResourceView);
		
		// **** Rendu de l’objet
		pImmediateContext->Draw( 6, 0 );

		time++;
	}

}
