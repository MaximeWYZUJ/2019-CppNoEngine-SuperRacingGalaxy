#pragma once

#include <string>
#include <d3d11.h>
#include "Device.h"

namespace SmallEngine
{
	class DeviceD3D11;

	class Texture
	{
	public:
		Texture();
		Texture(const std::wstring& fileName, Device* device);
		~Texture();
		std::wstring const& GetFilename() const noexcept;
		ID3D11ShaderResourceView* GetD3DTexture() const noexcept;
	private:
		Device* device;
		std::wstring fileName;
		ID3D11ShaderResourceView* texture;
		
	};
}
