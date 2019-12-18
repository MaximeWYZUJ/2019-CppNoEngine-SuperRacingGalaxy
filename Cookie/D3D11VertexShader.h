#pragma once
#include <d3d11.h>

namespace Cookie
{
	struct D3D11VPShader
	{
		ID3D11VertexShader* vertexShader;
		ID3D11InputLayout* inputLayout;
		ID3D11Buffer* constantBuffer;

		ID3D11PixelShader* pixelShader;
		ID3D11SamplerState* sampler;
	};
}
