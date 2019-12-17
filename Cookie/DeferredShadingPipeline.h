#pragma once
#include "Device.h"

namespace Cookie
{
	class DeferredShadingPipeline
	{
	public:
		static constexpr int32_t NbBuffers = 2;
		
		DeferredShadingPipeline(Device* device);
		~DeferredShadingPipeline();
		
		void Begin();
		void End();

	private:
		Device* device;
		
		std::vector<Device::Texture2D> renderTargetTextures;
		std::vector<Device::RenderTarget> renderTargets;
		std::vector<Device::ShaderResource> shaderResources;
	};
}
