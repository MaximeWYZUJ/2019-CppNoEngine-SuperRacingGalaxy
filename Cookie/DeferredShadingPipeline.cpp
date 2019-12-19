#include "pch.h"
#include "DeferredShadingPipeline.h"
#include "DeviceD3D11.h"
#include "Color.h"
#include "Util.h"

namespace Cookie
{
	DeferredShadingPipeline::DeferredShadingPipeline(Device* device, int width, int height)
		: device(device), firstPass(device)
	{
		/*auto d = dynamic_cast<DeviceD3D11*>(device)->GetD3DDevice();
		
		D3D11_TEXTURE2D_DESC textureDesc;
		D3D11_TEXTURE2D_DESC depthBufferDesc;
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;

		ZeroMemory(&textureDesc, sizeof(textureDesc));
		ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

		depthBufferDesc.Width = width;
		depthBufferDesc.Height = height;
		depthBufferDesc.MipLevels = 1;
		depthBufferDesc.ArraySize = 1;
		depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthBufferDesc.SampleDesc.Count = 1;
		depthBufferDesc.SampleDesc.Quality = 0;
		depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthBufferDesc.CPUAccessFlags = 0;
		depthBufferDesc.MiscFlags = 0;
		ThrowOnError(d->CreateTexture2D(&depthBufferDesc, nullptr, &depthStencilBuffer));
		
		ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;
		ThrowOnError(d->CreateDepthStencilView(depthStencilBuffer, &depthStencilViewDesc, &depthStencilView));

		viewport.Width = static_cast<float>(width);
		viewport.Height = static_cast<float>(height);
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;
		
		renderTargetTextures.reserve(NbBuffers);
		renderTargets.reserve(NbBuffers);
		shaderResources.reserve(NbBuffers);
		
		for (int i = 0; i < NbBuffers; ++i)
		{
			renderTargetTextures.push_back(device->CreateTexture2D({ static_cast<float>(width), static_cast<float>(height) }));
			renderTargets.push_back(device->CreateRenderTarget(renderTargetTextures[i]));
			shaderResources.push_back(device->CreateShaderResource(renderTargetTextures[i]));
		}*/
	}

	DeferredShadingPipeline::~DeferredShadingPipeline()
	{
		/*for (int i = 0; i < NbBuffers; ++i)
		{
			device->Release(shaderResources[i]);
			device->Release(renderTargets[i]);
			device->Release(renderTargetTextures[i]);
		}

		depthStencilView->Release();
		depthStencilBuffer->Release();*/
	}
	
	void DeferredShadingPipeline::BeginFirstPass()
	{
		/*ID3D11DeviceContext* context;
		auto d = dynamic_cast<DeviceD3D11*>(device)->GetD3DDevice();
		d->GetImmediateContext(&context);
		
		device->SetRenderTargets(renderTargets);
		context->RSSetViewports(1, &viewport);*/
	}
	
	void DeferredShadingPipeline::EndFirstPass()
	{
		//ID3D11DeviceContext* context;
		//auto d = dynamic_cast<DeviceD3D11*>(device)->GetD3DDevice();
		//d->GetImmediateContext(&context);
		//
		//device->ClearRenderTargets(renderTargets, Color::Black);

		//context->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
		////d->Get
	}
}
