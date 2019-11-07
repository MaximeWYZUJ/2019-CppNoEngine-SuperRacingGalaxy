#pragma once
#include "ExportMacro.h"

namespace Cookie
{
	class Material;
	class Device;
	struct ShadersParams;

	class COOKIE_API Shaders
	{
	public:
		Shaders(Device* device);
		void Activate(const DirectX::XMMATRIX& matWorld, const DirectX::XMMATRIX& matViewProj, const Material* mat) const;

	private:
		Device* device;

		ID3D11VertexShader* pVertexShader;
		ID3D11PixelShader* pPixelShader;
		ID3D11InputLayout* pVertexLayout;
		ID3D11Buffer* pConstantBuffer;
	};
}
