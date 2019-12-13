#pragma once

#include "ExportMacro.h"
#include "Matrix4x4.h"

struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;
struct ID3D11InputLayout;
struct ID3D11SamplerState;
struct ID3D11Buffer;

namespace Cookie
{
	class Material;
	class Device;

	class COOKIE_API Shaders
	{
	public:
		Shaders(Device* device);
		void Activate(const Matrix4x4<>& matWorld, const Matrix4x4<>& matViewProj, Vector3<> const& camPos, const Material* mat) const;

	private:
		Device* device;

		ID3D11VertexShader* pVertexShader;
		ID3D11PixelShader* pPixelShader;
		ID3D11InputLayout* pVertexLayout;
		ID3D11SamplerState* pSamplerState;
		ID3D11Buffer* pConstantBuffer;
	};
}
