#pragma once
#include <d3d11.h>
#include "Matrix4x4.h"
#include "Device.h"

namespace Cookie
{
	class DeferredShader
	{
	public:
		struct DeferredShaderParams
		{
			Matrix4x4<> worldMatrix;
			Matrix4x4<> projViewWorldMatrix;
		};

		DeferredShader(Device* device);

		void Activate(DeferredShaderParams const& p);
	private:
		Device* device;
		
		ID3D11VertexShader* vertexShader;
		ID3D11PixelShader* pixelShader;
		ID3D11InputLayout* inputLayout;
		ID3D11SamplerState* samplerState;
		ID3D11Buffer* matrixBuffer;
	};
}
