#include "pch.h"
#include "Texture.h"
#include "DDSTextureLoader.h"
#include "util.h"
#include "DeviceD3D11.h"
#include "resource.h"

namespace SmallEngine
{
	using namespace std;
	using namespace DirectX;

	Texture::Texture()
	{
	}
	
	Texture::Texture(const std::wstring& fileName, Device* device)
		: fileName { fileName }, device { device }
	{
		ID3D11Device* d = dynamic_cast<DeviceD3D11*>(device)->GetD3DDevice();
		// Charger la texture en ressource
		DXEssayer(CreateDDSTextureFromFile(d,
		fileName.c_str(),
		nullptr,
		&texture), DXE_FICHIERTEXTUREINTROUVABLE);
	}

	Texture::~Texture()
	{
		DXRelacher(texture);
	}
	
	wstring const& Texture::GetFilename() const noexcept
	{
		return fileName;
	}
	
	ID3D11ShaderResourceView* Texture::GetD3DTexture() const noexcept
	{
		return texture;
	}
}
