#pragma once
#include "Device.h"
#include "DeferredShader.h"
#include "DeferredLightingShader.h"

namespace Cookie
{
	class DeferredShadingPipeline
	{
	public:
		static constexpr int32_t NbBuffers = 2;
		
		DeferredShadingPipeline(Device* device, int width, int height);
		~DeferredShadingPipeline();
		
		void BeginFirstPass();
		void EndFirstPass();

	private:
		Device* device;

		DeferredShader firstPass;
		DeferredLightingShader secondPass;

		ID3D11Texture2D* depthStencilBuffer;
		ID3D11DepthStencilView* depthStencilView;
		D3D11_VIEWPORT viewport;
		std::vector<Device::Texture2D> renderTargetTextures;
		std::vector<Device::RenderTarget> renderTargets;
		std::vector<Device::ShaderResource> shaderResources;
	};
}
