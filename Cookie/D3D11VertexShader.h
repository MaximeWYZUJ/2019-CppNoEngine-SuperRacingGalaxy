#pragma once
#include "Shader.h"
#include <d3d11.h>

namespace Cookie
{
	class D3D11VertexShader : public Shader
	{
	public:
		D3D11VertexShader();
		virtual ~D3D11VertexShader();
	private:
		ID3D11VertexShader* vertexShader;
	};
}
