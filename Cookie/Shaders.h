#pragma once

#include "ExportMacro.h"
#include "Matrix4x4.h"

struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;
struct ID3D11InputLayout;
struct ID3D11SamplerState;
struct ID3D11Buffer;
struct ID3D11ShaderResourceView;
struct D3D11_INPUT_ELEMENT_DESC;

namespace Cookie
{
	struct ShadersParams;
	class Material;
	class Device;

	class COOKIE_API Shaders
	{
	public:
		Shaders(Device* device, const std::wstring& shaderName, UINT paramSize, D3D11_INPUT_ELEMENT_DESC* layout, int32_t nmElements);
		void Activate(ShadersParams* sp, ID3D11ShaderResourceView* texture) const;

	private:
		Device* device;

		ID3D11VertexShader* pVertexShader;
		ID3D11PixelShader* pPixelShader;
		ID3D11InputLayout* pVertexLayout;
		ID3D11SamplerState* pSamplerState;
		ID3D11Buffer* pConstantBuffer;
	};
}
