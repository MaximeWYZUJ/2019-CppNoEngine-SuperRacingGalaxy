#pragma once

#include "ExportMacro.h"

namespace SmallEngine
{
	class Device;
	struct ShadersParams;

	class COOKIE_API Material
	{
	public:
		Material(Device* device);
		void Activate(ShadersParams const& params) const;
	private:
		Device* device;
		
		ID3D11VertexShader* pVertexShader;
		ID3D11PixelShader* pPixelShader;
		ID3D11InputLayout* pVertexLayout;
		ID3D11Buffer* pConstantBuffer;
	};
}
