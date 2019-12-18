#pragma once
#include <d3d11.h>

namespace Cookie
{
	struct D3D11PixelShader
	{
		ID3D11PixelShader* shader;
		ID3D11SamplerState* sampler;
	};
}