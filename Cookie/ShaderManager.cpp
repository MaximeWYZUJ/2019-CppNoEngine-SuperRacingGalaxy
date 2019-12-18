#include "pch.h"
#include "ShaderManager.h"
#include "resource.h"
#include "Util.h"
#include "DeviceD3D11.h"
#include "Layout.h"
#include <locale>
#include <codecvt>
#include <string>
#include "D3D11PixelShader.h"

namespace Cookie
{
	using namespace std;
	
	ShaderManager::ShaderManager(Device* device)
		: device(device)
	{
	}

	auto ShaderManager::GetVPShader(std::string const& shaderName) -> D3D11VPShader*
	{
		auto it = nameToShaders.find(shaderName);
		if (it != end(nameToShaders))
		{
			return &vpShaders[it->second];
		}

		string vEntryPoint = shaderName + "VS";
		string t = vEntryPoint + ".hlsl";
		std::wstring vFileName = wstring(begin(t), end(t));

		ID3D11Device* d = static_cast<DeviceD3D11*>(device)->GetD3DDevice();
		ID3DBlob* pVSBlob = nullptr;
		DXEssayer(D3DCompileFromFile(vFileName.c_str(),
			nullptr, nullptr,
			vEntryPoint.c_str(),
			"vs_5_0",
			D3DCOMPILE_ENABLE_STRICTNESS,
			0,
			&pVSBlob, nullptr), DXE_FICHIER_VS);

		D3D11VPShader vp{};
		DXEssayer(d->CreateVertexShader(pVSBlob->GetBufferPointer(),
			pVSBlob->GetBufferSize(),
			nullptr,
			&vp.vertexShader),
			DXE_CREATION_VS);

		DXEssayer(d->CreateInputLayout(VertexData::layout,
			VertexData::nbElements,
			pVSBlob->GetBufferPointer(),
			pVSBlob->GetBufferSize(),
			&vp.inputLayout),
			DXE_CREATIONLAYOUT);
		pVSBlob->Release();

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof bd);
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof MatrixBuffer;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;
		d->CreateBuffer(&bd, nullptr, &vp.constantBuffer);

		string pEntryPoint = shaderName + "PS";
		t = pEntryPoint + ".hlsl";
		std::wstring pFileName = wstring(begin(t), end(t));
		
		ID3DBlob* pPSBlob = nullptr;
		DXEssayer(D3DCompileFromFile(pFileName.c_str(),
			nullptr, nullptr,
			pEntryPoint.c_str(),
			"ps_5_0",
			D3DCOMPILE_ENABLE_STRICTNESS,
			0,
			&pPSBlob,
			nullptr), DXE_FICHIER_PS);

		DXEssayer(d->CreatePixelShader(pPSBlob->GetBufferPointer(),
			pPSBlob->GetBufferSize(),
			nullptr,
			&vp.pixelShader),
			DXE_CREATION_PS);
		pPSBlob->Release();

		D3D11_SAMPLER_DESC desc;
		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.MipLODBias = 0.0f;
		desc.MaxAnisotropy = 1;
		desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		desc.BorderColor[0] = 0;
		desc.BorderColor[1] = 0;
		desc.BorderColor[2] = 0;
		desc.BorderColor[3] = 0;
		desc.MinLOD = 0;
		desc.MaxLOD = D3D11_FLOAT32_MAX;
		d->CreateSamplerState(&desc, &vp.sampler);

		vpShaders.push_back(vp);

		nameToShaders.insert({ shaderName, vpShaders.size() - 1 });

		return &vpShaders.back();
	}
}
