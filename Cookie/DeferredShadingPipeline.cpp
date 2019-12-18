#include "pch.h"
#include "DeferredShadingPipeline.h"
#include "DeviceD3D11.h"
#include "Color.h"

namespace Cookie
{
	DeferredShadingPipeline::DeferredShadingPipeline(Device* device)
		: device(device)
	{
		renderTargetTextures.reserve(NbBuffers);
		renderTargets.reserve(NbBuffers);
		shaderResources.reserve(NbBuffers);
		
		for (int i = 0; i < NbBuffers; ++i)
		{
			renderTargetTextures[i] = device->CreateTexture2D({ 1024, 768 });
			renderTargets[i] = device->CreateRenderTarget(renderTargetTextures[i]);
			shaderResources[i] = device->CreateShaderResource(renderTargetTextures[i]);
		}
	}

	DeferredShadingPipeline::~DeferredShadingPipeline()
	{
		for (int i = 0; i < NbBuffers; ++i)
		{
			device->Release(shaderResources[i]);
			device->Release(renderTargets[i]);
			device->Release(renderTargetTextures[i]);
		}
	}
	
	void DeferredShadingPipeline::Begin()
	{
		device->SetRenderTargets(renderTargets);
	}
	
	void DeferredShadingPipeline::End()
	{
		device->ClearRenderTargets(renderTargets, Color::Black);
	}
}
